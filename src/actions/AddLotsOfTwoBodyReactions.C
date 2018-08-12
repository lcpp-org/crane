#include "AddLotsOfTwoBodyReactions.h"
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

registerMooseAction("CraneApp", AddLotsOfTwoBodyReactions, "add_material");
registerMooseAction("CraneApp", AddLotsOfTwoBodyReactions, "add_kernel");
registerMooseAction("CraneApp", AddLotsOfTwoBodyReactions, "add_bc");

template <>
InputParameters
validParams<AddLotsOfTwoBodyReactions>()
{
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());

  InputParameters params = validParams<AddVariableAction>();
  params.addRequiredParam<std::vector<NonlinearVariableName>>(
    "species", "List of (tracked) species included in reactions (both products and reactants)");
  params.addParam<bool>("include_electrons", false, "Whether or not electrons are being considered.");
  params.addParam<bool>("track_energy", false, "Whether or not to track (any) species energy.");
  params.addParam<std::vector<NonlinearVariableName>>(
    "species_energy", "List of (tracked) energy values. (Optional; requires 'track_energy' to be True.)");
  params.addParam<std::string>("electron_density", "The variable used for density of electrons.");
  params.addParam<std::vector<VariableName>>(
    "electron_energy", "Electron energy, used for energy-dependent reaction rates.");
  params.addParam<std::vector<std::string>>("gas_species", "All of the background gas species in the system.");
  params.addParam<std::vector<Real>>("gas_fraction", "The initial fraction of each gas species.");
  params.addParam<bool>("gas_tracking", false, "If false, neutral gas is treated as uniform background (_n_gas).");
  params.addParam<std::vector<VariableName>>("potential", "The electric potential, used for energy-dependent reaction rates.");
  params.addRequiredParam<std::string>("reactions", "The list of reactions to be added");
  params.addParam<Real>("position_units", 1.0, "The units of position.");
  params.addRequiredParam<std::string>("reaction_coefficient_format",
    "The format of the reaction coefficient. Options: rate or townsend.");
  params.addParam<std::string>("file_location", "", "The location of the reaction rate files. Default: empty string (current directory).");
  params.addParam<bool>("use_moles", "Whether to use molar units.");
  params.addClassDescription("This Action automatically adds the necessary kernels and materials for a reaction network.");

  return params;
}

// Here are a few functions for removing whitespace before/after expressions.
// (Makes the reaction input formatting more forgiving!)
static inline string &ltrim(string &s)
{
  s.erase(s.begin(),find_if_not(s.begin(),s.end(),[](int c){return isspace(c);}));
  return s;
}

static inline string &rtrim(string &s)
{
  s.erase(find_if_not(s.rbegin(),s.rend(),[](int c){return isspace(c);}).base(), s.end());
  return s;
}

static inline string trim(string &s)
{
  return ltrim(rtrim(s));
}

AddLotsOfTwoBodyReactions::AddLotsOfTwoBodyReactions(InputParameters params)
  : Action(params),
    _species(getParam<std::vector<NonlinearVariableName>>("species")),
    _species_energy(getParam<std::vector<NonlinearVariableName>>("species_energy")),
    _input_reactions(getParam<std::string>("reactions")),
    _r_units(getParam<Real>("position_units")),
    _coefficient_format(getParam<std::string>("reaction_coefficient_format"))
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
  std::vector<std::string> _rate_equation_string;

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
    bool energy_change = false;
    pos = token.find(':'); // Looks for colon, which separates reaction and rate coefficients

    // Brackets enclose the electron energy gain/loss (if applicable)
    pos_start = token.find('[');
    pos_end = token.find(']');

    eq_start = token.find('{');
    eq_end = token.find('}');

    _reaction.push_back(token.substr(0, pos)); // Stores reactions
    rate_coefficient_string.push_back(token.substr(pos+1, pos_start - (pos+1)));

    trim(_reaction[counter]);
    trim(rate_coefficient_string[counter]);

    if (pos_start != std::string::npos)
    {
      threshold_energy_string.push_back(token.substr(pos_start + 1, pos_end-pos_start-1));
      energy_change = true;
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
      _rate_equation.push_back(false);
    }
    counter += 1;
  }

  _num_reactions = _reaction.size();
  _rate_coefficient.resize(_num_reactions, 0);
  _threshold_energy.resize(_num_reactions, 0);

  _elastic_collision.resize(_num_reactions, false);

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
      _threshold_energy[i] = stof(threshold_energy_string[i]);
    }

    if (rate_coefficient_string[i] == std::string("BOLOS"))
    {
      _rate_coefficient[i] = NAN;
    }
    else if (_rate_equation[i] == true)
    {
      _rate_coefficient[i] = NAN;
    }
    else
    {
      _rate_coefficient[i] = stof(rate_coefficient_string[i]);
    }
  }
  _reaction_coefficient_name.resize(_num_reactions);
  _reactants.resize(_num_reactions);
  _products.resize(_num_reactions);
  _species_count.resize(_num_reactions, std::vector<Real>(_species.size()));
  _superelastic_reaction.resize(_num_reactions);
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
        _superelastic_reaction[i] = false;
        react_check = false;
        counter = 0;
        continue;
      }
      else if (token == "<=>" || token == "<->")
      {
        superelastic_reactions += 1;
        _superelastic_reaction[i] = true;

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

  // _num_reactions = _num_reactions + superelastic_reactions;
  // _superelastic_index.resize(superelastic_reactions);
  std::string new_reaction;

  if (superelastic_reactions > 0)
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      if (_superelastic_reaction[i] == true)
      {
        _superelastic_index.push_back(i);

        // Here we reverse the products and reactants to build superelastic reactions.
        for (unsigned int j = 0; j < _num_products[i]; ++j)
        {
          new_reaction.append(_products[i][j]);
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
        for (unsigned int j = 0; j < _num_reactants[i]; ++j)
        {
          new_reaction.append(_reactants[i][j]);
          if (j == _num_reactants[i] - 1)
          {
            break;
          }
          else
          {
            new_reaction.append(" + ");
          }
        }
        std::cout << _reaction[i] << std::endl;
        std::cout << new_reaction << std::endl;
      }
    }
  }

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
AddLotsOfTwoBodyReactions::act()
{

  int v_index;
  bool find_other;
  bool species_v, species_w;
  unsigned int target; // stores index of target species for electron-impact reactions
  std::string product_kernel_name;
  std::string reactant_kernel_name;
  std::string energy_kernel_name;
  std::vector<NonlinearVariableName> variables =
      getParam<std::vector<NonlinearVariableName>>("species");
  // std::vector<VariableName> electron_energy =
      // getParam<std::vector<VariableName>>("electron_energy");
  // std::string electron_density = getParam<std::string>("electron_density");

  bool gas_tracking = getParam<bool>("gas_tracking");
  std::vector<std::string> gas_species = getParam<std::vector<std::string>>("gas_species");

  if (gas_tracking)
  {
    mooseError("Functionality for tracking neutral gas densities and temperatures is under development.");
  }

  if (_current_task == "add_material")
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      _reaction_coefficient_name[i] = "alpha_"+_reaction[i];
      if (isnan(_rate_coefficient[i]) && _rate_equation[i] == false)
      {
        Real position_units = getParam<Real>("position_units");
        InputParameters params = _factory.getValidParams("GenericEnergyDependentReactionRate");
        params.set<std::string>("reaction") = _reaction[i];
        params.set<std::string>("file_location") = getParam<std::string>("file_location");
        params.set<Real>("position_units") = position_units;
        params.set<std::vector<VariableName>>("em") = {_reactants[i][_electron_index[i]]};
        params.set<std::vector<VariableName>>("mean_en") = getParam<std::vector<VariableName>>("electron_energy");
        params.set<std::string>("reaction_coefficient_format") = _coefficient_format;

        // This section determines if the target species is a tracked variable.
        // If it isn't, the target is assumed to be the background gas (_n_gas).
        // (This cannot handle gas mixtures yet -- base code needs to be modified to
        // distinguish between different gas densities for that to work.)
        bool target_species_tracked = false;
        for (unsigned int j = 0; j < _species.size(); ++j)
        {
          // Checking for the target species in electron-impact reactions, so
          // electrons are ignored.
          if (getParam<bool>("use_electrons") == true)
          {
            if (_species[j] == getParam<std::string>("electron_density"))
            {
              continue;
            }
          }

          for (unsigned int k = 0; k < _reactants[i].size(); ++k)
          {
            if (_reactants[i][k] == _species[j])
            {
              target_species_tracked = true;
              target = k;
              break;
            }
          }

          if (target_species_tracked)
            break;
        }
        if (target_species_tracked)
        {
          params.set<std::vector<VariableName>>("target_species") = {_reactants[i][target]};
        }
        params.set<bool>("elastic_collision") = {_elastic_collision[i]};
        params.set<FileName>("property_file") = "reaction_"+_reaction[i]+".txt";

        _problem->addMaterial("GenericEnergyDependentReactionRate", "reaction_"+std::to_string(i), params);
      }
      else
      {
        InputParameters params = _factory.getValidParams("GenericReactionRate");
        params.set<std::string>("reaction") = _reaction[i];
        params.set<Real>("reaction_rate_value") = _rate_coefficient[i];
        _problem->addMaterial("GenericReactionRate", "reaction_"+std::to_string(i), params);
      }

    }
  }

  // Add appropriate kernels to each reactant and product.
  if (_current_task == "add_kernel")
  {
    int index; // stores index of species in the reactant/product arrays
    std::vector<std::string>::iterator iter;
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      if (!isnan(_rate_coefficient[i]))
      {
        if (_num_reactants[i] == 1)
        {
          product_kernel_name = "ProductFirstOrder";
          reactant_kernel_name = "ReactantFirstOrder";
        }
        else if (_num_reactants[i] == 2)
        {
        product_kernel_name = "ProductSecondOrder";
        reactant_kernel_name = "ReactantSecondOrder";
        }
        else
        {
          mooseError("LotsOfTwoBodyReactions cannot handle "+std::to_string(_num_reactants[i])+"-body reactions! \nReaction: "+_reaction[i]);
        }
      }
      else
      {
        if (_coefficient_format == "townsend")
        {
          product_kernel_name = "ElectronImpactReactionProduct";
          reactant_kernel_name = "ElectronImpactReactionReactant";
          energy_kernel_name = "ElectronEnergyTerm";

          // Add energy equation source/sink term(s)
          InputParameters params = _factory.getValidParams(energy_kernel_name);
          params.set<NonlinearVariableName>("variable") = _species_energy[0];
          if (_coefficient_format == "townsend")
            params.set<std::vector<VariableName>>("potential") = getParam<std::vector<VariableName>>("potential");
          params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
          params.set<std::string>("reaction") = _reaction[i];
          params.set<Real>("threshold_energy") = _threshold_energy[i];
          params.set<Real>("position_units") = _r_units;
          _problem->addKernel(energy_kernel_name, "energy_kernel"+std::to_string(i)+"_"+_reaction[i], params);
        }
        else
        {
          iter = std::find(_reactants[i].begin(), _reactants[i].end(), getParam<std::string>("electron_density"));
          index = std::distance(_reactants[i].begin(), iter);
          v_index = std::abs(index - 1);
          find_other = std::find(_species.begin(), _species.end(), _reactants[i][v_index]) != _species.end();


          // product_kernel_name = "ProductABRxn";
          // reactant_kernel_name = "ReactantABRxn2";
          energy_kernel_name = "ElectronEnergyTermRate";

          InputParameters params = _factory.getValidParams(energy_kernel_name);
          params.set<NonlinearVariableName>("variable") = _species_energy[0];
          params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
          if (find_other)
            params.set<std::vector<VariableName>>("v") = {_reactants[i][v_index]};
          params.set<std::string>("reaction") = _reaction[i];
          params.set<Real>("threshold_energy") = _threshold_energy[i];
          params.set<Real>("position_units") = _r_units;
          _problem->addKernel(energy_kernel_name, "energy_kernel"+std::to_string(i)+"_"+_reaction[i], params);
        }
      }

      for (int j = 0; j < _species.size(); ++j)
      {
        iter = std::find(_reactants[i].begin(), _reactants[i].end(), _species[j]);
        index = std::distance(_reactants[i].begin(), iter);

        if (iter != _reactants[i].end())
        {
          // Now we see if the second reactant is a tracked species.
          // We only treat two-body reactions now. This will need to be changed for three-body reactions.
          // e.g. 1) find size of reactants array 2) use find() to search other values inside that size that are not == index
          // 3) same result!
          v_index = std::abs(index - 1);
          find_other = std::find(_species.begin(), _species.end(), _reactants[i][v_index]) != _species.end();
          if (_species_count[i][j] < 0)
          {
            if (!isnan(_rate_coefficient[i]))
            {
              InputParameters params = _factory.getValidParams(reactant_kernel_name);
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<Real>("coefficient") = _species_count[i][j];
              params.set<std::string>("reaction") = _reaction[i];

              if (find_other)
              {
                params.set<std::vector<VariableName>>("v") = {_reactants[i][v_index]};
              }
              _problem->addKernel(reactant_kernel_name, "kernel"+std::to_string(j)+"_"+_reaction[i], params);
            }
            else
            {
              if (_coefficient_format == "townsend")
              {
                InputParameters params = _factory.getValidParams(reactant_kernel_name);
                // params.set<NonlinearVariableName>("variable") = _reactants[i][index];
                params.set<NonlinearVariableName>("variable") = _species[j];
                params.set<std::vector<VariableName>>("mean_en") = getParam<std::vector<VariableName>>("electron_energy");
                if (_coefficient_format == "townsend")
                  params.set<std::vector<VariableName>>("potential") = getParam<std::vector<VariableName>>("potential");
                params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
                params.set<Real>("position_units") = _r_units;
                params.set<std::string>("reaction") = _reaction[i];
                params.set<std::string>("reaction_coefficient_name") = _reaction_coefficient_name[i];
                _problem->addKernel(reactant_kernel_name, "kernel"+std::to_string(j)+"_"+_reaction[i], params);
              }
              else if (_coefficient_format == "rate")
              {
                InputParameters params = _factory.getValidParams(reactant_kernel_name);
                params.set<NonlinearVariableName>("variable") = _species[j];
                params.set<Real>("coefficient") = _species_count[i][j];
                params.set<std::string>("reaction") = _reaction[i];

                if (find_other)
                {
                  params.set<std::vector<VariableName>>("v") = {_reactants[i][v_index]};
                }
                _problem->addKernel(reactant_kernel_name, "kernel"+std::to_string(j)+"_"+_reaction[i], params);

              }
            }
          }
        }

        // Now we do the same thing for the products side of the reaction
        iter = std::find(_products[i].begin(), _products[i].end(), _species[j]);
        index = std::distance(_products[i].begin(), iter);
        species_v = std::find(_species.begin(), _species.end(), _reactants[i][0]) != _species.end();
        species_w = std::find(_species.begin(), _species.end(), _reactants[i][1]) != _species.end();

        if (iter != _products[i].end())
        {

          if (_species_count[i][j] > 0)
          {
            if (!isnan(_rate_coefficient[i]))
            {
              InputParameters params = _factory.getValidParams(product_kernel_name);
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<std::string>("reaction") = _reaction[i];
              if (species_v)
                params.set<std::vector<VariableName>>("v") = {_reactants[i][0]};
              if (species_w)
                params.set<std::vector<VariableName>>("w") = {_reactants[i][1]};
              params.set<Real>("coefficient") = _species_count[i][j];
              _problem->addKernel(product_kernel_name, "kernel_prod"+std::to_string(j)+"_"+_reaction[i], params);
            }
            else
            {
              if (_coefficient_format == "townsend")
              {
                InputParameters params = _factory.getValidParams(product_kernel_name);
                params.set<NonlinearVariableName>("variable") = _species[j];
                params.set<std::vector<VariableName>>("mean_en") = getParam<std::vector<VariableName>>("electron_energy");
                if (_coefficient_format == "townsend")
                  params.set<std::vector<VariableName>>("potential") = getParam<std::vector<VariableName>>("potential");
                params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
                params.set<Real>("position_units") = _r_units;
                params.set<std::string>("reaction") = _reaction[i];
                params.set<std::string>("reaction_coefficient_name") = _reaction_coefficient_name[i];
                _problem->addKernel(product_kernel_name, "kernel_prod"+std::to_string(j)+"_"+_reaction[i], params);
              }
              else if (_coefficient_format == "rate")
              {
                InputParameters params = _factory.getValidParams(product_kernel_name);
                params.set<NonlinearVariableName>("variable") = _species[j];
                params.set<std::string>("reaction") = _reaction[i];
                if (species_v)
                  params.set<std::vector<VariableName>>("v") = {_reactants[i][0]};
                if (species_w)
                  params.set<std::vector<VariableName>>("w") = {_reactants[i][1]};
                params.set<Real>("coefficient") = _species_count[i][j];
                _problem->addKernel(product_kernel_name, "kernel_prod"+std::to_string(j)+"_"+_reaction[i], params);
              }
            }
          }
        }

      }
    }
  }

}
