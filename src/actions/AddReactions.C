#include "AddReactions.h"
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

registerMooseAction("CraneApp", AddReactions, "add_aux_variable");
registerMooseAction("CraneApp", AddReactions, "add_aux_kernel");
registerMooseAction("CraneApp", AddReactions, "add_material");
registerMooseAction("CraneApp", AddReactions, "add_kernel");
registerMooseAction("CraneApp", AddReactions, "add_function");

template <>
InputParameters
validParams<AddReactions>()
{
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());

  InputParameters params = validParams<ChemicalReactionsBase>();
  params.addParam<std::vector<SubdomainName>>("block",
                                              "The subdomain that this action applies to.");
  params.addClassDescription(
      "This Action automatically adds the necessary kernels and materials for a reaction network.");

  return params;
}

static inline std::string &
ltrim(std::string & s)
{
  s.erase(s.begin(), find_if_not(s.begin(), s.end(), [](int c) { return isspace(c); }));
  return s;
}

static inline std::string &
rtrim(std::string & s)
{
  s.erase(find_if_not(s.rbegin(), s.rend(), [](int c) { return isspace(c); }).base(), s.end());
  return s;
}

static inline std::string
trim(std::string & s)
{
  return ltrim(rtrim(s));
}

AddReactions::AddReactions(InputParameters params) : ChemicalReactionsBase(params)
{
  if (_use_log)
    _log_append = "Log";
  else
    _log_append = "";

  // Define reactant names for kernels
  // (In kernels, coupled variables are typically referred to as "v", "w", "x", etc...)
  _reactant_names.resize(3);
  _reactant_names[0] = "v";
  _reactant_names[1] = "w";
  _reactant_names[2] = "x";
}

void
AddReactions::act()
{
  int v_index;
  std::vector<int> other_index;
  std::vector<int> reactant_indices;
  std::vector<std::string> other_variables;
  other_variables.resize(3);
  other_variables[0] = "v";
  other_variables[1] = "w";
  other_variables[2] = "x";
  bool find_other;
  bool find_aux;
  std::vector<bool> include_species;
  unsigned int target; // stores index of target species for electron-impact reactions
  std::string product_kernel_name;
  std::string reactant_kernel_name;
  std::string energy_kernel_name;
  // std::vector<NonlinearVariableName> variables =
  //     getParam<std::vector<NonlinearVariableName>>("species");
  // std::vector<VariableName> electron_energy =
  // getParam<std::vector<VariableName>>("electron_energy");
  // std::string electron_density = getParam<std::string>("electron_density");

  if (_current_task == "add_aux_variable")
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      auto var_params = _factory.getValidParams("MooseVariableConstMonomial");
      //_problem->addAuxVariable(_aux_var_name[i], FIRST);
      _problem->addAuxVariable("MooseVariableConstMonomial", _aux_var_name[i], var_params);
    }
  }

  if (_current_task == "add_aux_kernel")
  {
    // This boolean is only for EEDF reactions. It is always set to false for every other reaction
    // type.
    if (_track_rates)
    {
      std::string kernel_name;

      // For AuxKernels function and constant-based reactions are identical.
      // They are added separately just by convention.
      // getKernelName(_num_reactants, is_energy, is_aux)
      for (unsigned int i = 0; i < _num_function_reactions; ++i)
      {
        kernel_name = getReactionKernelName(_reactants[_function_reaction_number[i]].size(), true);
        addAuxRate(kernel_name, _function_reaction_number[i]);
      }

      for (unsigned int i = 0; i < _num_constant_reactions; ++i)
      {
        kernel_name =
            getReactionKernelName(_reactants[_constant_reaction_number[i]].size(), true);
        addAuxRate(kernel_name, _constant_reaction_number[i]);
      }
    }
  }

  if (_current_task == "add_material")
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      if (_rate_type[i] == "Constant")
      {
        addConstantCoefficient(i);
      }
      else if (_rate_type[i] == "Equation")
      {
        /*
         * THIS WILL NEED TO BE MODIFIED.
         * ADD COUPLED NONLINEAR VARIABLES THAT NEED
         * DERIVATIVES. NO AD CAPABILITY YET SO THIS
         * WILL NEED TO BE DONE MANUALLY.
         */
        addFunctionCoefficient(i);
      }
      else if (_superelastic_reaction[i] == true)
      {
        mooseError("Superelastic reactions not supported yet!");
        // addSuperelasticCoefficient(i);
      }
    }
  }

  // Add appropriate kernels to each reactant and product.
  if (_current_task == "add_kernel")
  {
    // Initialize the kernel name
    std::string kernel_name;

    /*
     *
     * FUNCTION REACTIONS
     *
     * (Note that functions will be added as normal kernels, not AD.
     * No AD functionality exists for parsed materials.)
     */
    for (unsigned int i = 0; i < _num_function_reactions; ++i)
    {
      for (unsigned int j = 0; j < _species.size(); ++j)
      {
        kernel_name = getReactionKernelName(_reactants[_function_reaction_number[i]].size(), false);
        if (_species_count[_function_reaction_number[i]][j] != 0)
        {
          addFunctionReaction(_function_reaction_number[i], j, kernel_name);
        }
        else
          continue;
      }
    }

    /*
     *
     * CONSTANT REACTIONS
     *
     */
    for (unsigned int i = 0; i < _num_constant_reactions; ++i)
    {
      for (unsigned int j = 0; j < _species.size(); ++j)
      {
        kernel_name = getReactionKernelName(_reactants[_constant_reaction_number[i]].size(), false);
        if (_species_count[_constant_reaction_number[i]][j] != 0)
        {
          addConstantReaction(_constant_reaction_number[i], j, kernel_name);
        }
        else
          continue;
      }
    }
  }
}

void
AddReactions::addConstantCoefficient(const unsigned & reaction_num)
{
  InputParameters params = _factory.getValidParams("GenericRateConstant");
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<Real>("reaction_rate_value") = _rate_coefficient[reaction_num];
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  params.set<std::string>("number") = Moose::stringify(reaction_num);
  _problem->addMaterial("GenericRateConstant",
                        "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                            std::to_string(reaction_num) + "_" + _name,
                        params);
}

void
AddReactions::addFunctionCoefficient(const unsigned & reaction_num)
{
  InputParameters params = _factory.getValidParams("DerivativeParsedMaterial");
  params.set<std::string>("f_name") =
      "k" + Moose::stringify(reaction_num) + "_" + _reaction[reaction_num];
  params.set<std::vector<VariableName>>("args") =
      getParam<std::vector<VariableName>>("equation_variables");
  params.set<std::vector<std::string>>("constant_names") =
      getParam<std::vector<std::string>>("equation_constants");
  params.set<std::vector<std::string>>("constant_expressions") =
      getParam<std::vector<std::string>>("equation_values");
  params.set<std::string>("function") = _rate_equation_string[reaction_num];
  params.set<unsigned int>("derivative_order") = 1;
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  _problem->addMaterial("DerivativeParsedMaterial",
                        "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                            std::to_string(reaction_num) + "_" + _name,
                        params);
}

void
AddReactions::addConstantReaction(const unsigned & reaction_num,
                                  const unsigned & species_num,
                                  const std::string & kernel_name)
{
  std::string kernel_identifier;

  InputParameters params = _factory.getValidParams(kernel_name);
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("number") = Moose::stringify(reaction_num);
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  params.set<NonlinearVariableName>("variable") = _species[species_num];
  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    params.set<std::vector<VariableName>>(_reactant_names[k]) = {_reactants[reaction_num][k]};
    if (_species[species_num] == _reactants[reaction_num][k])
    {
      params.set<bool>("_" + _reactant_names[k] + "_eq_u") = true;
    }
  }
  params.set<Real>("coefficient") = _species_count[reaction_num][species_num];
  kernel_identifier = "kernel_constant_" + getParam<std::vector<SubdomainName>>("block")[0] +
                      std::to_string(reaction_num) + "_" + std::to_string(species_num);

  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  _problem->addKernel(kernel_name, kernel_identifier + "_" + _name, params);
}

void
AddReactions::addFunctionReaction(const unsigned & reaction_num,
                                  const unsigned & species_num,
                                  const std::string & kernel_name)
{
  // Source and sink terms that are dictated by a parsed function rate coefficient are little tricky
  // to handle, so they require a separate kernel function. As of the writing of this code no
  // automatic differentiation capabilities exist for parsed materials, so the jacobians need to be
  // included manually. Luckily parsed rate coefficients almost always only exist in terms of
  // electron or gas temperature, but in its current form this still requires computing the jacobian
  // of the parsed function with respect to AT LEAST electrons and _mean_en.
  std::string kernel_identifier;

  InputParameters params = _factory.getValidParams(kernel_name);
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("number") = Moose::stringify(reaction_num);
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  // define the coupled variables in the function to compute jacobian contribution
  // params.set<std::vector<VariableName>>("args") =
  //    getParam<std::vector<VariableName>>("equation_variables");

  // TO DO: (1) INCLUDE VARIABLES FOR JACOBIAN CONTRIBUTION OF RATE COEFFICIENT
  //        (2) ALLOW FOR BOTH ELECTRON AND GAS TEMPERATURE

  params.set<NonlinearVariableName>("variable") = _species[species_num];
  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    params.set<std::vector<VariableName>>(_reactant_names[k]) = {_reactants[reaction_num][k]};
    if (_species[species_num] == _reactants[reaction_num][k])
    {
      params.set<bool>("_" + _reactant_names[k] + "_eq_u") = true;
    }
  }
  params.set<Real>("coefficient") = _species_count[reaction_num][species_num];
  kernel_identifier = "kernel_function_" + getParam<std::vector<SubdomainName>>("block")[0] +
                      std::to_string(reaction_num) + "_" + std::to_string(species_num);

  _problem->addKernel(kernel_name, kernel_identifier + "_" + _name, params);
}

std::string
AddReactions::getReactionKernelName(const unsigned & num_reactants, const bool & is_aux)
{
  /*
   * This function only gets kernel names for non-eedf kernels.
   * There are three types of kernels (First-, Second-, and Third-order reactions)
   * Each has variants for (a) log formulation (b) linear formulation.
   */
  std::string name = "Reaction";
  if (is_aux)
    name += "Rate";

  if (num_reactants == 1)
    name += "FirstOrder";
  if (num_reactants == 2)
    name += "SecondOrder";
  if (num_reactants == 3)
    name += "ThirdOrder";

  return (name + _log_append);
}

void
AddReactions::addAuxRate(const std::string & aux_kernel_name,
                               const unsigned & reaction_num)
{

  auto params = _factory.getValidParams(aux_kernel_name);

  // Add the reactants. _reactant_names = ["v", "w", "x"], just like for regular kernels
  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    params.set<std::vector<VariableName>>(_reactant_names[k]) = {_reactants[reaction_num][k]};
  }
  params.set<std::string>("number") = Moose::stringify(reaction_num);

  params.set<std::string>("reaction") = _reaction[reaction_num];
  //params.set<AuxVariableName>("variable") = {"rate" + std::to_string(reaction_num) + "_" + _name};
  params.set<AuxVariableName>("variable") = {_aux_var_name[reaction_num]};
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_END";
  _problem->addAuxKernel(
      aux_kernel_name, "Calc_Reaction_Rate" + std::to_string(reaction_num) + "_" + _name, params);
}
