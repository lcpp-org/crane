#include "AddScalarReactions.h"
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

registerMooseAction("CraneApp", AddScalarReactions, "add_aux_variable");
registerMooseAction("CraneApp", AddScalarReactions, "add_aux_scalar_kernel");
registerMooseAction("CraneApp", AddScalarReactions, "add_scalar_kernel");
registerMooseAction("CraneApp", AddScalarReactions, "add_function");
registerMooseAction("CraneApp", AddScalarReactions, "add_user_object");

template <>
InputParameters
validParams<AddScalarReactions>()
{
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());

  InputParameters params = validParams<ChemicalReactionsBase>();
  params.addParam<std::vector<std::string>>("aux_species", "Auxiliary species that are not included in nonlinear solve.");
  params.addClassDescription("This Action automatically adds the necessary kernels and materials for a reaction network.");
  return params;
}

AddScalarReactions::AddScalarReactions(InputParameters params)
  : ChemicalReactionsBase(params),
    _aux_species(getParam<std::vector<std::string>>("aux_species"))
{
}

void
AddScalarReactions::act()
{
  int v_index;
  std::vector<int> other_index;
  std::vector<int> reactant_indices;

  // Allocate space for all three reactants. (Used as a check to see if the other
  // reactants are tracked; if they are, they are passed as coupled variables.)
  // Note: possible to replace with 'args' as a coupled variable array?
  std::vector<std::string> other_variables;
  other_variables.resize(3);
  other_variables[0] = "v";
  other_variables[1] = "w";
  other_variables[2] = "x";
  bool find_other;
  bool find_aux;

  std::vector<bool> include_species;
  std::string product_kernel_name;
  std::string reactant_kernel_name;
  std::string energy_kernel_name;
  // std::vector<NonlinearVariableName> variables =
  //     getParam<std::vector<NonlinearVariableName>>("species");

  bool gas_tracking = getParam<bool>("gas_tracking");
  std::vector<std::string> gas_species = getParam<std::vector<std::string>>("gas_species");

  if (gas_tracking)
  {
    // mooseError("Functionality for tracking neutral gas densities and temperatures is under development.");
    std::cout << "WARNING: Functionality for tracking neutral gas densities is still under development." << std::endl;
  }

  if (_current_task == "add_aux_variable")
  {
    for (unsigned int i=0; i < _num_reactions; ++i)
    {
      _problem->addAuxScalarVariable(_aux_var_name[i], FIRST);
    }
  }

  if (_current_task == "add_user_object")
  {
    for (unsigned int i=0; i < _num_reactions; ++i)
    {
      // If this particular reaction is not reversible, skip to the next one.
      // If it is, we add the necessary user object to calculate the 7-term
      // polynomial expansion.
      // (This just sets the coefficients. The actual equilibrium constant
      // is calculated through an auxiliary variable, as all other rate coefficients are.)
      if (!_reversible_reaction[i])
        continue;
      else
      {
        InputParameters params = _factory.getValidParams("PolynomialCoefficients");
        params.set<std::vector<Real>>("stoichiometric_coeff") = _reaction_stoichiometric_coeff[i];
        params.set<std::vector<std::string>>("participants") = _reaction_participants[i];
        params.set<std::string>("file_location") = "PolynomialCoefficients";
        params.set<ExecFlagEnum>("execute_on") = "INITIAL";
        _problem->addUserObject("PolynomialCoefficients", "superelastic_coeff"+std::to_string(i), params);
      }
    }
  }

  if (_current_task == "add_aux_scalar_kernel")
  {
    for (unsigned int i=0; i < _num_reactions; ++i)
    {
      if (_rate_type[i] == "EEDF" && !_superelastic_reaction[i])
      {
        InputParameters params = _factory.getValidParams("DataReadScalar");
        params.set<AuxVariableName>("variable") = {_aux_var_name[i]};
        params.set<std::vector<VariableName>>("sampler") = {"reduced_field"};
        params.set<FileName>("property_file") = "reaction_"+_reaction[i]+".txt";
        params.set<std::string>("file_location") = getParam<std::string>("file_location");
        params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
        _problem->addAuxScalarKernel("DataReadScalar", "aux_rate"+std::to_string(i), params);
      }
      else if (_rate_type[i] == "Equation" && !_superelastic_reaction[i])
      {
        InputParameters params = _factory.getValidParams("ParsedScalarRateCoefficient");
        params.set<AuxVariableName>("variable") = {_aux_var_name[i]};
        params.set<std::string>("function") = _rate_equation_string[i];
        params.set<bool>("file_read") = true;
        // params.set<std::vector<std::string>>("file_value") = {"Te"};
        params.set<std::vector<std::string>>("constant_names") = getParam<std::vector<std::string>>("equation_constants");
        params.set<std::vector<std::string>>("constant_expressions") = getParam<std::vector<std::string>>("equation_values");
        // params.set<UserObjectName>("electron_temperature") = "value_provider";
        // params.set<std::vector<VariableName>>("reduced_field") = {"reduced_field"};
        if (getParam<bool>("gas_temperature"))
        {
          params.set<bool>("gas_temperature") = true;
          std::vector<NonlinearVariableName> gas_temp = getParam<std::vector<NonlinearVariableName>>("equation_variables");
          std::string temp_var;
          for (unsigned int m = 0; m<gas_temp.size(); ++m)
          {
            if (gas_temp[m] == "Tgas")
              temp_var = gas_temp[m];
              break;
          }
          params.set<std::vector<VariableName>>("args") = {temp_var};
          // params.set<std::vector<VariableName>>("args") = getParam<std::vector<VariableName>>("equation_variables");

        }
        // params.set<std::vector<VariableName>>("args") = {"Te"};
        params.set<std::vector<VariableName>>("args") = getParam<std::vector<VariableName>>("equation_variables");
        // params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN NONLINEAR";
        params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
        _problem->addAuxScalarKernel("ParsedScalarRateCoefficient", "aux_rate"+std::to_string(i), params);
      }
      else if (_rate_type[i] == "Constant" && !_superelastic_reaction[i])
      {
        InputParameters params = _factory.getValidParams("AuxInitialConditionScalar");
        params.set<Real>("initial_condition") = _rate_coefficient[i];
        params.set<AuxVariableName>("variable") = {_aux_var_name[i]};
        params.set<ExecFlagEnum>("execute_on") = "INITIAL";
        _problem->addAuxScalarKernel("AuxInitialConditionScalar", "aux_initialization_rxn"+std::to_string(i), params);
      }
      else if (_superelastic_reaction[i])
      {
        InputParameters params = _factory.getValidParams("SuperelasticRateCoefficientScalar");
        params.set<AuxVariableName>("variable") = {_aux_var_name[i]};
        params.set<std::vector<VariableName>>("forward_coefficient") = {_aux_var_name[_superelastic_index[i]]};
        params.set<Real>("Tgas_const") = 300;
        params.set<UserObjectName>("polynomial_provider") = "superelastic_coeff"+std::to_string(_superelastic_index[i]);
        params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
        _problem->addAuxScalarKernel("SuperelasticRateCoefficientScalar", "aux_rate"+std::to_string(i), params);
      }
    }
  }

  if (_current_task == "add_scalar_kernel")
  {
    int index; // stores index of species in the reactant/product arrays
    std::vector<std::string>::iterator iter;
    std::vector<std::string>::iterator iter_aux;
    std::vector<Real> rxn_coeff = getParam<std::vector<Real>>("reaction_coefficient");
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      if (_reactants[i].size() == 1)
      {
        product_kernel_name = "Product1BodyScalar";
        reactant_kernel_name = "Reactant1BodyScalar";
      }
      else if (_reactants[i].size() == 2)
      {
        product_kernel_name = "Product2BodyScalar";
        reactant_kernel_name = "Reactant2BodyScalar";
      }
      else
      {
        product_kernel_name = "Product3BodyScalar";
        reactant_kernel_name = "Reactant3BodyScalar";
      }
      if (_use_log)
      {
        product_kernel_name += "Log";
        reactant_kernel_name += "Log";
      }

      if (_energy_change[i] && _rate_type[i] != "EEDF")
      {
        int non_electron_index;
        // find_other = std::find(_species.begin(), _species.end(), _reactants[i][v_index]) != _species.end();
        // Coupled variable must be generalized to allow for 3 reactants
        InputParameters params = _factory.getValidParams(energy_kernel_name);
        params.set<NonlinearVariableName>("variable") = _species_energy[0];
        params.set<std::vector<VariableName>>("em") = {"em"};
        // Find the non-electron reactant
        for (unsigned int k=0; k<_reactants[i].size(); ++k)
        {
          if (_reactants[i][k] == "em")
            continue;
          else
            non_electron_index = k;
        }
        // Check if value is tracked, and if so, add as coupled variable.
        find_other = std::find(_species.begin(), _species.end(), _reactants[i][non_electron_index]) != _species.end();
        find_aux = std::find(_aux_species.begin(), _aux_species.end(), _reactants[i][non_electron_index]) != _aux_species.end();
        if (find_other || find_aux)
          params.set<std::vector<VariableName>>("v") = {_reactants[i][non_electron_index]};

        // params.set<std::vector<VariableName>>("v") = {"Ar*"};
        params.set<std::string>("reaction") = _reaction[i];
        params.set<Real>("threshold_energy") = _threshold_energy[i];
        params.set<Real>("position_units") = _r_units;
        _problem->addKernel(energy_kernel_name, "energy_kernel"+std::to_string(i)+"_"+_reaction[i], params);
      }

      // Find any aux variables in the species list.
      // If found, that index is skipped in the next loop.
      for (int j = 0; j < _species.size(); ++j)
      {
        iter = std::find(_reactants[i].begin(), _reactants[i].end(), _species[j]);
        index = std::distance(_reactants[i].begin(), iter);

        iter_aux = std::find(_aux_species.begin(), _aux_species.end(), _species[j]);
        if (iter_aux != _aux_species.end())
        {
          continue;
        }
        if (iter != _reactants[i].end())
        {
          reactant_indices.resize(_reactants[i].size());
          for (unsigned int k=0; k<_reactants[i].size(); ++k)
            reactant_indices[k] = k;
          reactant_indices.erase(reactant_indices.begin() + index);
          for (unsigned int k=0; k<reactant_indices.size(); ++k)
          {
            find_other = std::find(_species.begin(), _species.end(), _reactants[i][reactant_indices[k]]) != _species.end();
            if (find_other)
              continue;
            else
              reactant_indices.erase(reactant_indices.begin() + k);
          }
          v_index = std::abs(index - 1);
          find_other = std::find(_species.begin(), _species.end(), _reactants[i][v_index]) != _species.end();
          if (_species_count[i][j] < 0)
          {
            InputParameters params = _factory.getValidParams(reactant_kernel_name);
            params.set<NonlinearVariableName>("variable") = _species[j];
            params.set<Real>("coefficient") = _species_count[i][j];
            params.set<Real>("n_gas") = 3.219e18;
            params.set<std::vector<VariableName>>("rate_coefficient") = {_aux_var_name[i]};
            params.set<bool>("rate_constant_equation") = true;
            if (find_other)
            {
              for (unsigned int k=0; k<reactant_indices.size(); ++k)
                params.set<std::vector<VariableName>>(other_variables[k]) = {_reactants[i][reactant_indices[k]]};
            }
            _problem->addScalarKernel(reactant_kernel_name, "kernel"+std::to_string(j)+"_"+_reaction[i], params);

          }
        }

        iter = std::find(_products[i].begin(), _products[i].end(), _species[j]);
        include_species.resize(_reactants[i].size());
        for (unsigned int k=0; k<_reactants[i].size(); ++k)
        {
          include_species[k] = std::find(_species.begin(), _species.end(), _reactants[i][k]) != _species.end();
        }
        if (iter != _products[i].end())
        {

          if (_species_count[i][j] > 0)
          {
            InputParameters params = _factory.getValidParams(product_kernel_name);
            params.set<NonlinearVariableName>("variable") = _species[j];
            params.set<Real>("n_gas") = 3.219e18;
            params.set<std::vector<VariableName>>("rate_coefficient") = {_aux_var_name[i]};
            params.set<bool>("rate_constant_equation") = true;
            params.set<Real>("coefficient") = _species_count[i][j];
            for (unsigned int k=0; k<_reactants[i].size(); ++k)
            {
              if (include_species[k])
              {
                params.set<std::vector<VariableName>>(other_variables[k]) = {_reactants[i][k]};
                if (_species[j] == _reactants[i][k])
                {
                  params.set<bool>(other_variables[k]+"_eq_u") = true;
                }
              }

            }
            _problem->addScalarKernel(product_kernel_name, "kernel_prod"+std::to_string(j)+"_"+_reaction[i], params);
          }
        }

      }
    }
  }
}
