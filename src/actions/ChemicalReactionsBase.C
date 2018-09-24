#include "ChemicalReactionsBase.h"
#include "Parser.h"
#include "FEProblem.h"
#include "Factory.h"
#include "MooseEnum.h"
#include "AddVariableAction.h"
#include "Conversion.h"
#include "DirichletBC.h"
#include "ActionFactory.h"
#include "MooseObjectAction.h"
#include "MooseApp.h"

#include "libmesh/vector_value.h"

#include "pcrecpp.h"

#include <sstream>
#include <stdexcept>

// libmesh includes
#include "libmesh/libmesh.h"
#include "libmesh/exodusII_io.h"
#include "libmesh/equation_systems.h"
#include "libmesh/nonlinear_implicit_system.h"
#include "libmesh/explicit_system.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/fe.h"

registerMooseAction("CraneApp", ChemicalReactionsBase, "add_aux_variable");
registerMooseAction("CraneApp", ChemicalReactionsBase, "add_aux_scalar_kernel");
registerMooseAction("CraneApp", ChemicalReactionsBase, "add_material");
registerMooseAction("CraneApp", ChemicalReactionsBase, "add_kernel");
registerMooseAction("CraneApp", ChemicalReactionsBase, "add_scalar_kernel");
registerMooseAction("CraneApp", ChemicalReactionsBase, "add_function");

template <>
InputParameters
validParams<ChemicalReactionsBase>()
{
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());

  InputParameters params = validParams<AddVariableAction>();
  params.addRequiredParam<std::vector<NonlinearVariableName>>(
    "species", "List of (tracked) species included in reactions (both products and reactants)");
  params.addParam<std::vector<Real>>("reaction_coefficient", "The reaction coefficients.");
  params.addParam<bool>("include_electrons", false, "Whether or not electrons are being considered.");
  params.addParam<bool>("track_energy", false, "Whether or not to track gas energy/temperature.");
  params.addParam<bool>("track_electron_energy", false, "Whether or not to track electron energy.");
  params.addParam<bool>("use_log", false, "Whether or not to use logarithmic densities. (N = exp(n))");
  params.addParam<std::vector<NonlinearVariableName>>(
    "species_energy", "List of (tracked) energy values. (Optional; requires 'track_energy' to be True.)");
  params.addParam<std::string>("electron_density", "The variable used for density of electrons.");
  params.addParam<std::vector<VariableName>>(
    "electron_energy", "Electron energy, used for energy-dependent reaction rates.");
  params.addParam<std::vector<std::string>>("gas_species", "All of the background gas species in the system.");
  params.addParam<std::vector<Real>>("gas_fraction", "The initial fraction of each gas species.");
  params.addParam<bool>("gas_tracking", false, "If false, neutral gas is treated as uniform background (_n_gas).");
  params.addParam<bool>("gas_temperature", false, "If false, neutral gas temperature is not a solution variable.");
  params.addParam<std::vector<VariableName>>("gas_temperature_variable", "The gas temperature variable (if applicable).");
  params.addParam<std::vector<VariableName>>("potential", "The electric potential, used for energy-dependent reaction rates.");
  params.addRequiredParam<std::string>("reactions", "The list of reactions to be added");
  params.addParam<Real>("position_units", 1.0, "The units of position.");
  params.addParam<std::string>("file_location", "", "The location of the reaction rate files. Default: empty string (current directory).");
  params.addParam<bool>("use_moles", "Whether to use molar units.");
  params.addParam<std::string>("sampling_format", "reduced_field", "Sample rate constants with E/N (reduced_field) or Te (electron_energy).");
  params.addParam<bool>("scalar_problem", false, "The problem is scalar if it is a pure ODE problem (Global/0D).");
  params.addParam<std::vector<std::string>>("equation_constants", "The constants included in the reaction equation(s).");
  params.addParam<std::vector<std::string>>("equation_values", "The values of the constants included in the reaction equation(s).");
  params.addParam<std::vector<NonlinearVariableName>>("equation_variables", "Any nonlinear variables that appear in the equations.");
  params.addParam<std::vector<VariableName>>("rate_provider_var", "The name of the variable used to sample from BOLOS files.");
  params.addClassDescription("This Action automatically adds the necessary kernels and materials for a reaction network.");

  return params;
}

// Here are a few functions for removing whitespace before/after expressions.
// (Makes the reaction input formatting more forgiving!)
// static inline string &ltrim(string &s)
// {
//   s.erase(s.begin(),find_if_not(s.begin(),s.end(),[](int c){return isspace(c);}));
//   return s;
// }
//
// static inline string &rtrim(string &s)
// {
//   s.erase(find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base(), s.end());
//   return s;
// }
//
// static inline string trim(string &s)
// {
//   return ltrim(rtrim(s));
// }

ChemicalReactionsBase::ChemicalReactionsBase(InputParameters params)
  : Action(params),
    _species(getParam<std::vector<NonlinearVariableName>>("species")),
    _species_energy(getParam<std::vector<NonlinearVariableName>>("species_energy")),
    _input_reactions(getParam<std::string>("reactions")),
    _r_units(getParam<Real>("position_units")),
    _sampling_format(getParam<std::string>("sampling_format")),
    _use_log(getParam<bool>("use_log")),
    _scalar_problem(getParam<bool>("scalar_problem"))
    // _scalar_problem(getParam<bool>("scalar_problem"))
{
  // 1) split into reactants and products
  // 2) split products into products and reaction rate
  // 3) check reaction rate; if constant, store. If "BOLOS", read data file.
  // 4) split reactants and products into individual species
  // 5) Apply appropriate kernels and/or materials. (In the act() function.)

  std::istringstream iss(_input_reactions);
  std::string token;
  std::string token2;
  std::vector<std::string> rate_coefficient_string;
  std::vector<std::string> threshold_energy_string;
  // std::vector<std::string> _rate_equation_string;

  size_t pos;
  size_t pos_start;
  size_t pos_end;
  size_t eq_start;
  size_t eq_end;
  int counter;
  counter = 0;
  while (std::getline(iss >> std::ws, token)) // splits by \n character (default) and ignores leading whitespace
  {
    // Define check for change of energy
    bool _energy_change = false;
    pos = token.find(':'); // Looks for colon, which separates reaction and rate coefficients

    // Brackets enclose the energy gain/loss (if applicable)
    pos_start = token.find('[');
    pos_end = token.find(']');

    // Curly braces enclose equation constants (Arrhenius form)
    eq_start = token.find('{');
    eq_end = token.find('}');

    _reaction.push_back(token.substr(0, pos)); // Stores reactions
    rate_coefficient_string.push_back(token.substr(pos+1, pos_start - (pos+1)));

    trim(_reaction[counter]);
    trim(rate_coefficient_string[counter]);

    if (pos_start != std::string::npos)
    {
      threshold_energy_string.push_back(token.substr(pos_start + 1, pos_end-pos_start-1));
      _energy_change = true;
    }
    else
    {
      threshold_energy_string.push_back("\0");
    }

    if (eq_start != std::string::npos)
    {
      _rate_equation_string.push_back(token.substr(eq_start + 1, eq_end-eq_start-1));
      _rate_equation.push_back(true);
    }
    else
    {
      _rate_equation_string.push_back("NONE");
      _rate_equation.push_back(false);
    }
    counter += 1;
  }

  _num_reactions = _reaction.size();
  _rate_coefficient.resize(_num_reactions, 0);
  _threshold_energy.resize(_num_reactions, 0);

  _elastic_collision.resize(_num_reactions, false);
  _rate_type.resize(_num_reactions);
  _aux_var_name.resize(_num_reactions);
  for (unsigned int i = 0; i < _num_reactions; ++i)
  {
    if (threshold_energy_string[i] == "\0")
    {
      _threshold_energy[i] = 0.0;
    }
    else if (threshold_energy_string[i] == "elastic")
    {
      _threshold_energy[i] = 0.0;
      _elastic_collision[i] = true;
    }
    else
    {
      _threshold_energy[i] = std::stod(threshold_energy_string[i]);
    }
    _aux_var_name[i] = "rate_constant"+std::to_string(i);  // Stores name of rate coefficients
    if (rate_coefficient_string[i] == std::string("BOLOS"))
    {
      _rate_coefficient[i] = NAN;
      _rate_type[i] = "EEDF";
    }
    else if (_rate_equation[i] == true)
    {
      _rate_coefficient[i] = NAN;
      _rate_type[i] = "Equation";
      // USE THIS CODE TO SEE IF VARIABLE IS CONTAINED WITHIN EQUATION
      //////////
      // if (_rate_equation_string[i].find("Tgas") != std::string::npos)
      // {
      //   std::cout << "found!" << std::endl;
      // }
      //////////

      // Here we need to parse the individual reaction equations to find the
      // variables and constants.
      // std::istringstream iss(_rate_equation_string[i]);
      // std::string token;
      // while (std::getline(iss >> std::ws, token,'/'))
      // {
      //   std::cout << token << std::endl;
      // }
    }
    else
    {
      _rate_coefficient[i] = std::stod(rate_coefficient_string[i]);
      _rate_type[i] = "Constant";
    }
  }

  _reactants.resize(_num_reactions);
  _products.resize(_num_reactions);
  _reversible_reaction.resize(_num_reactions);
  _electron_index.resize(_num_reactions, 0);
  // _species_electron.resize(_num_reactions, std::vector<bool>(_species.size()));

  // Split each reaction equation into reactants and products
  int superelastic_reactions = 0;  // stores number of superelastic reactions, which will be added to _num_reactions
  for (unsigned int i = 0; i < _num_reactions; ++i)
  {
    std::istringstream iss2(_reaction[i]);
    std::string token;

    // Isolate individual terms of each reaction
    unsigned int counter = 0;
    bool react_check = true;
    while (std::getline(iss2 >> std::ws, token, ' '))
    {
      // Check for non-variable entries. Skip the plus signs, and if an equals
      // sign shows up we switch from reactants to products.
      // (This is a pretty hacky way to do this...but it works.)
      if (token == "+")
      {
        // If "+", skip to next iteration
        continue;
      }
      else if (token == "=" || token == "->" || token == "=>")
      {
        // If "=", switch from reactants to products, reset counter, and then
        // skip to next iteration.
        _reversible_reaction[i] = false;
        react_check = false;
        counter = 0;
        continue;
      }
      else if (token == "<=>" || token == "<->")
      {
        superelastic_reactions += 1;
        _reversible_reaction[i] = true;

        react_check = false;
        counter = 0;
        continue;
        // mooseError("Cannot handle superelastic reactions yet. Add two separate reactions.");
      }

      // Check if we are on the left side (reactants) or right side (products)
      // of the reaction equation.
      _all_participants.push_back(token);
      if (react_check)
      {
        _reactants[i].push_back(token);
      }
      else
      {
        _products[i].push_back(token);
      }
      counter = counter + 1;
    }

    _num_reactants.push_back(_reactants[i].size());
    _num_products.push_back(_products[i].size());
    _species_count.resize(_num_reactions, std::vector<Real>(_species.size()));
    for (unsigned int j = 0; j < _species.size(); ++j)
    {
      for (unsigned int k = 0; k < _reactants[i].size(); ++k)
      {
        if (_reactants[i][k] == _species[j])
        {
          _species_count[i][j] -= 1;
        }
        if (getParam<bool>("include_electrons") == true)
        {
          if (_reactants[i][k] == getParam<std::string>("electron_density"))
          {
            _electron_index[i] = k;
          }
        }
      }
      for (unsigned int k = 0; k < _products[i].size(); ++k)
      {
        if (_products[i][k] == _species[j])
        {
          _species_count[i][j] += 1;
        }
      }
    }
  }

  std::string new_reaction;
  int new_index = _num_reactions - 1;

  // We also need to resize rate_coefficient and threshold_energy to account
  // for the new reaction(s)
  _superelastic_index.resize(_num_reactions + superelastic_reactions, 0);
  _superelastic_reaction.resize(_num_reactions + superelastic_reactions, false);
  _rate_coefficient.resize(_num_reactions + superelastic_reactions);
  _threshold_energy.resize(_num_reactions + superelastic_reactions);
  _rate_equation.resize(_num_reactions + superelastic_reactions);
  _species_count.resize(_num_reactions + superelastic_reactions, std::vector<Real>(_species.size()));
  _reactants.resize(_reactants.size() + superelastic_reactions);
  _products.resize(_products.size() + superelastic_reactions);
  _aux_var_name.resize(_num_reactions + superelastic_reactions);
  if (superelastic_reactions > 0)
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      if (_reversible_reaction[i] == true)
      {
        // _reaction.resize(_num_reactions + 1);
        new_index += 1;
        // This index refers to the ORIGINAL reaction. Example:
        // If reaction #2 out of 5 (index = 1 from [0,4]) is reversible, then a
        // 6th reaction (index = 5) will be added. This is the superelastic reaction.
        // _superelastic_index is intended to refer back to the original reversible
        // reaction later in the code, so we can reverse any energy change and refer
        // to the forward reaction rate if necessary. Thus in this particular case,
        // _superelastic_index[5] = 1.
        _superelastic_index[new_index] = i;
        _superelastic_reaction[new_index] = true;
        _rate_coefficient[new_index] = NAN;
        _threshold_energy[new_index] = -_threshold_energy[i];
        _aux_var_name[new_index] = "rate_constant"+std::to_string(i);
        if (_rate_equation[i] == true)
        {
          _rate_equation[new_index] = true;
        }
        else
        {
          _rate_equation[new_index] = false;
        }

        // Here we reverse the products and reactants to build superelastic reactions.
        for (unsigned int j = 0; j < _num_products[i]; ++j)
        {
          new_reaction.append(_products[i][j]);
          _reactants[new_index].push_back(_products[i][j]);
          if (j == _num_products[i] - 1)
          {
            break;
          }
          else
          {
            new_reaction.append(" + ");
          }
        }
        new_reaction.append(" -> ");
        for (unsigned int j = 0; j < _reactants[i].size(); ++j)
        {
          new_reaction.append(_reactants[i][j]);
          _products[new_index].push_back(_reactants[i][j]);
          if (j == _reactants[i].size() - 1)
          {
            break;
          }
          else
          {
            new_reaction.append(" + ");
          }
        }
        _reaction.push_back(new_reaction);
      }

      // Calculate coefficients
      for (unsigned int j = 0; j < _species.size(); ++j)
      {
        for (unsigned int k = 0; k < _reactants[i].size(); ++k)
        {
          if (_reactants[i][k] == _species[j])
          {
            _species_count[i][j] -= 1;
          }
          if (getParam<bool>("include_electrons") == true)
          {
            if (_reactants[i][k] == getParam<std::string>("electron_density"))
            {
              _electron_index[i] = k;
            }
          }
        }
        for (unsigned int k = 0; k < _products[i].size(); ++k)
        {
          if (_products[i][k] == _species[j])
          {
            _species_count[i][j] += 1;
          }
        }
      }
    }
  }

  _num_reactions += superelastic_reactions;
  _reaction_coefficient_name.resize(_num_reactions);
  // Find the unique species across all reaction pathways
  // Note that this also accounts for species that are not tracked in case
  // some of the species are considered to be uniform background gases or
  // arbitrary source/sink terms.
  sort(_all_participants.begin(), _all_participants.end());
  std::vector<std::string>:: iterator it;
  it = std::unique(_all_participants.begin(), _all_participants.end());
  _all_participants.resize(std::distance(_all_participants.begin(), it));

  // Find the stoichometric coefficient for each participant.
  _stoichiometric_coeff.resize(_reaction.size());

  for (unsigned int i = 0; i < _reaction.size(); ++i)
  {
    _stoichiometric_coeff[i].resize(_all_participants.size(), 0);

    for (unsigned int j = 0; j < _all_participants.size(); ++j)
    {
      for (unsigned int k = 0; k < _reactants[i].size(); ++k)
      {
        if (_reactants[i][k] == _all_participants[j])
        {
          _stoichiometric_coeff[i][j] -= 1;
        }
      }

      for (unsigned int k = 0; k < _products[i].size(); ++k)
      {
        if (_products[i][k] == _all_participants[j])
        {
          _stoichiometric_coeff[i][j] += 1;
        }
      }


    }
  }

  // Now we find which index of _all_participants is associated with _species
  // so they can be accurately referred to later if necessary.

  _species_index.resize(_species.size()); // Stores vector of indices relating _all_participants to _species.
  std::vector<std::string>::iterator iter;

  for (unsigned int i = 0; i < _species.size(); ++i)
  {
    iter = std::find(_all_participants.begin(), _all_participants.end(), _species[i]);
    _species_index[i] = std::distance(_all_participants.begin(), iter);
  }

}

void
ChemicalReactionsBase::act()
{
}
