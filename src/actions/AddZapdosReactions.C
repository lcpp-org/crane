#include "AddZapdosReactions.h"
#include "Parser.h"
#include "FEProblem.h"
#include "Factory.h"
#include "MooseEnum.h"
#include "AddVariableAction.h"
#include "Conversion.h"
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

registerMooseAction("CraneApp", AddZapdosReactions, "add_aux_variable");
registerMooseAction("CraneApp", AddZapdosReactions, "add_aux_kernel");
registerMooseAction("CraneApp", AddZapdosReactions, "add_material");
registerMooseAction("CraneApp", AddZapdosReactions, "add_kernel");

template <>
InputParameters
validParams<AddZapdosReactions>()
{
  MooseEnum families(AddVariableAction::getNonlinearVariableFamilies());
  MooseEnum orders(AddVariableAction::getNonlinearVariableOrders());

  InputParameters params = validParams<ChemicalReactionsBase>();
  params.addRequiredParam<std::string>(
      "reaction_coefficient_format",
      "The format of the reaction coefficient. Options: rate or townsend.");
  params.addParam<std::vector<VariableName>>(
      "potential", "The electric potential, used for energy-dependent reaction rates.");
  params.addParam<std::vector<std::string>>(
      "aux_species", "Auxiliary species that are not included in nonlinear solve.");
  params.addParam<std::vector<SubdomainName>>("block",
                                              "The subdomain that this action applies to.");
  params.addParam<bool>(
      "use_ad",
      false,
      "Set to true if you want to use automatic differentiation. This comes at a slight "
      "computational cost, but Jacobians are guaranteed to be absolutely correct.");
  params.addClassDescription(
      "This Action automatically adds the necessary kernels and materials for a reaction network.");

  return params;
}

AddZapdosReactions::AddZapdosReactions(InputParameters params)
  : ChemicalReactionsBase(params),
    _coefficient_format(getParam<std::string>("reaction_coefficient_format")),
    _use_ad(getParam<bool>("use_ad"))

{
  if (_coefficient_format == "townsend")
  {
    // This adds the appropriate kernel name to EEDF-related reactions. Otherwise it is a blank
    // string and normal rate-based reactions will be used.
    _townsend_append = "Townsend";

    // Error checking - make sure electron density and energy are included for
    // townsend formulation
    // (Necessary for electron flux calculation)
    if (!isParamValid("electron_density"))
      mooseError(
          "Coefficient format type 'townsend' requires an input parameter 'electron_density'!");

    if (!isParamValid("electron_energy"))
      mooseError(
          "Coefficient format type 'townsend' requires an input parameter 'electron_energy'!");
  }
  else
    _townsend_append = "";

  if (_use_ad)
    _ad_prepend = "AD";
  else
    _ad_prepend = "";

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
AddZapdosReactions::act()
{

  // Add all rate constant as materials
  // One rate constant is added per reaction.
  if (_current_task == "add_material")
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      ////////////////////////////
      // WHAT IS THIS USED FOR? //
      ////////////////////////////
      _reaction_coefficient_name[i] = "alpha_" + _reaction[i];
      if (_rate_type[i] == "EEDF")
      {
        addEEDFCoefficient(i);
      }
      else if (_rate_type[i] == "Constant")
      {
        addConstantRateCoefficient(i);
      }
      else if (_rate_type[i] == "Equation")
      {
        /*
         * THIS WILL NEED TO BE MODIFIED.
         * ADD COUPLED NONLINEAR VARIABLES THAT NEED
         * DERIVATIVES. NO AD CAPABILITY YET SO THIS
         * WILL NEED TO BE DONE MANUALLY.
         */
        addFunctionRateCoefficient(i);
      }
      else if (_superelastic_reaction[i] == true)
      {
        mooseError("Superelastic reactions not supported yet!");
        addSuperelasticRateCoefficient(i);
      }
    }
  }

  // Here all kernels are added.
  // Each type of reaction (electron-impact, constant rate, function-based rate) are added
  // separately.
  if (_current_task == "add_kernel")
  {
    // Note that the reactions are numbered in the order they are written in
    // the input file, even though they may be added to the problem in a
    // different order.

    /*
     *
     * EEDF REACTIONS
     *
     */
    std::string kernel_name;
    for (unsigned int i = 0; i < _num_eedf_reactions; ++i)
    {
      // We need to find the non-electron species index
      int electron_index;
      int target_index;

      for (unsigned int kk = 0; kk < _reactants[_eedf_reaction_number[i]].size(); ++kk)
      {
        if (_reactants[_eedf_reaction_number[i]][kk] == getParam<std::string>("electron_density"))
          electron_index = kk;
        else
          target_index = kk;
      }
      for (unsigned int j = 0; j < _species.size(); ++j)
      {
        if (_species_count[_eedf_reaction_number[i]][j] != 0)
        {
          kernel_name = getElectronImpactKernelName(false, false, false);
          addEEDFKernel(_eedf_reaction_number[i], j, kernel_name, electron_index, target_index);
          //_ad_prepend + "ElectronImpactReaction" + _townsend_append + _log_append,
        }
      }

      if (_energy_change[_eedf_reaction_number[i]])
      {
        kernel_name =
            getElectronImpactKernelName(true, _elastic_collision[_eedf_reaction_number[i]], false);
        // This will be used to switch between gas temperature and electron temperature
        /*
        if (_electron_energy_term[t])
          energy_sign = 1.0;
        else
          energy_sign = -1.0;
        */
        addEEDFEnergy(_eedf_reaction_number[i], kernel_name);
      }
    }

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
        kernel_name = getKernelName(_reactants[_function_reaction_number[i]].size(), false, false);
        if (_species_count[_function_reaction_number[i]][j] != 0)
        {
          addFunctionKernel(_function_reaction_number[i], j, kernel_name, false);
        }
        else
          continue;
      }

      if (_energy_change[_function_reaction_number[i]])
      {
        kernel_name = getKernelName(_reactants[_function_reaction_number[i]].size(), true, false);
        addFunctionKernel(_function_reaction_number[i], 0, kernel_name, true);
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
        kernel_name = getKernelName(_reactants[_constant_reaction_number[i]].size(), false, false);
        if (_species_count[_constant_reaction_number[i]][j] != 0)
        {
          addConstantKernel(_constant_reaction_number[i], j, kernel_name, false);
        }
        else
          continue;
      }

      if (_energy_change[_constant_reaction_number[i]])
      {
        kernel_name = getKernelName(_reactants[_constant_reaction_number[i]].size(), true, false);
        addConstantKernel(_constant_reaction_number[i], 0, kernel_name, true);
      }
    }
  }

  /*
   * If the _track_rates option is set to true, AuxKernels and AuxVariables will be automatically
   * generated to track the reaction rate for each reaction (e.g. k*n1*n2). This may incur
   * significant computational cost to the simulation depending on the number of reactions and
   * number of nodes.
   */
  if (_current_task == "add_aux_variable")
  {
    if (_track_rates == true)
    {
      for (unsigned int i = 0; i < _num_reactions; ++i)
      {
        _problem->addAuxVariable(_aux_var_name[i], FIRST);
        auto params = _factory.getValidParams("MooseVariableConstMonomial");
        params.set<MooseEnum>("order") = "CONSTANT";
        params.set<MooseEnum>("family") = "MONOMIAL";
        params.set<std::vector<SubdomainName>>("block") =
            getParam<std::vector<SubdomainName>>("block");
        _problem->addAuxVariable(
            "MooseVariableConstMonomial", "rate" + std::to_string(i) + "_" + _name, params);
      }
    }
  }

  if (_current_task == "add_aux_kernel")
  {
    // This boolean is only for EEDF reactions. It is always set to false for every other reaction
    // type.
    if (_track_rates)
    {
      std::string kernel_name;

      // First all EEDF-based reactions are added.
      for (unsigned int i = 0; i < _num_eedf_reactions; ++i)
      {
        if (_coefficient_format == "townsend")
          addAuxRate(
              "ReactionRateEEDF" + _townsend_append + _log_append, _eedf_reaction_number[i], true);
        else
        {
          kernel_name = getKernelName(_reactants[_eedf_reaction_number[i]].size(), false, true);
          addAuxRate(kernel_name, _eedf_reaction_number[i], false);
        }
      }

      // For AuxKernels function and constant-based reactions are identical.
      // They are added separately just by convention.
      // getKernelName(_num_reactants, is_energy, is_aux)
      for (unsigned int i = 0; i < _num_function_reactions; ++i)
      {
        kernel_name = getKernelName(_reactants[_function_reaction_number[i]].size(), false, true);
        addAuxRate(kernel_name, _function_reaction_number[i], false);
      }

      for (unsigned int i = 0; i < _num_constant_reactions; ++i)
      {
        kernel_name = getKernelName(_reactants[_constant_reaction_number[i]].size(), false, true);
        addAuxRate(kernel_name, _constant_reaction_number[i], false);
      }
    }
  }
}

void
AddZapdosReactions::addAuxRate(const std::string & aux_kernel_name,
                               const unsigned & reaction_num,
                               const bool & is_townsend)
{

  auto params = _factory.getValidParams(aux_kernel_name);
  if (is_townsend)
  {
    // First we find the electron and target indices
    for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
    {
      if (_reactants[reaction_num][k] == getParam<std::string>("electron_density"))
        params.set<std::vector<VariableName>>("electrons") = {_reactants[reaction_num][k]};
      else
        params.set<std::vector<VariableName>>("target") = {_reactants[reaction_num][k]};
    }
    params.set<std::vector<VariableName>>("mean_energy") = {_electron_energy[0]};
    params.set<std::vector<VariableName>>("potential") =
        getParam<std::vector<VariableName>>("potential");
    params.set<std::string>("reaction_coefficient_name") = {"alpha" + std::to_string(reaction_num) +
                                                            "_" + _reaction[reaction_num]};
    params.set<Real>("position_units") = _r_units;
  }
  else
  {
    // Add the reactants. _reactant_names = ["v", "w", "x"], just like for regular kernels
    for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
    {
      params.set<std::vector<VariableName>>(_reactant_names[k]) = {_reactants[reaction_num][k]};
    }
    params.set<std::string>("number") = Moose::stringify(reaction_num);
  }

  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<AuxVariableName>("variable") = {"rate" + std::to_string(reaction_num) + "_" + _name};
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_END";
  _problem->addAuxKernel(
      aux_kernel_name, "Calc_Reaction_Rate" + std::to_string(reaction_num) + "_" + _name, params);
}

std::string
AddZapdosReactions::getElectronImpactKernelName(const bool & energy_kernel,
                                                const bool & elastic_kernel,
                                                const bool & is_aux)
{
  /*
   * This function only gets kernel names for eedf kernels.
   * Each has variants for (a) log formulation (b) linear formulation.
   * There are also choices for energy terms, including elastic collisions.
   */
  std::string name = "EEDF";

  if (energy_kernel)
  {
    if (elastic_kernel)
      name += "Elastic";
    else
      name += "Energy";
  }
  else
    name += "Reaction";

  if (is_aux)
    name += "Rate";

  return (_ad_prepend + name + _townsend_append + _log_append);
}

std::string
AddZapdosReactions::getKernelName(const unsigned & num_reactants,
                                  const bool & energy_kernel,
                                  const bool & is_aux)
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

  if (energy_kernel)
    name += "Energy";

  return (name + _log_append);
}

void
AddZapdosReactions::addEEDFKernel(const unsigned & reaction_num,
                                  const unsigned & species_num,
                                  const std::string & kernel_name,
                                  const int & electron_index,
                                  const int & target_index)
{
  auto params = _factory.getValidParams(kernel_name);
  params.set<NonlinearVariableName>("variable") = _species[species_num];

  // Necessary for jacobian, but we are sticking with AD for now
  // params.set<std::vector<VariableName>>("mean_en") = {_electron_energy[0]};

  // params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
  params.set<std::vector<VariableName>>("electrons") = {_reactants[reaction_num][electron_index]};
  params.set<std::vector<VariableName>>("target") = {_reactants[reaction_num][target_index]};
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  params.set<Real>("coefficient") = (Real)_species_count[reaction_num][species_num];
  params.set<std::string>("number") = Moose::stringify(reaction_num);

  // For townsend coefficients, potential variable is needed to compute electron flux
  if (_coefficient_format == "townsend")
  {
    params.set<std::vector<VariableName>>("potential") =
        getParam<std::vector<VariableName>>("potential");
    params.set<Real>("position_units") = _r_units;
  }

  if (_use_ad)
  {
    _problem->addKernel(kernel_name,
                        "kernel_eedf_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            std::to_string(reaction_num) + std::to_string(species_num) + "_" +
                            _name,
                        params);
    _problem->haveADObjects(true);
  }
  else
  {
    params.set<std::vector<VariableName>>("mean_energy") = {_electron_energy[0]};
    _problem->addKernel(kernel_name,
                        "kernel_eedf_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                            std::to_string(reaction_num) + "_" + std::to_string(species_num) + "_" +
                            _name,
                        params);
  }
}

void
AddZapdosReactions::addEEDFEnergy(const unsigned & reaction_num, const std::string & kernel_name)
{
  /*
   * Adds kernels for energy changes based on tabulated EEDF rate constants.
   * Note that this adds both elastic and inelastic collision energy changes.
   * Currently only applies to electron energy, not gas temperature.
   */
  unsigned int non_electron_index;
  std::string elastic;
  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    if (_reactants[reaction_num][k] == getParam<std::string>("electron_density"))
      continue;
    else
      non_electron_index = k;
  }

  InputParameters params = _factory.getValidParams(kernel_name);

  // If this is not an elastic collision, the change in enthalpy needs to be included
  if (!_elastic_collision[reaction_num])
    params.set<Real>("threshold_energy") = _threshold_energy[reaction_num];

  params.set<NonlinearVariableName>("variable") = {_electron_energy[0]};
  params.set<std::vector<VariableName>>("target") = {_reactants[reaction_num][non_electron_index]};

  params.set<std::vector<VariableName>>("target") = {_reactants[reaction_num][non_electron_index]};
  params.set<std::string>("reaction") = _reaction[reaction_num];

  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  params.set<std::string>("number") = Moose::stringify(reaction_num);

  if (_coefficient_format == "townsend")
  {
    params.set<Real>("position_units") = _r_units;
    params.set<std::vector<VariableName>>("potential") =
        getParam<std::vector<VariableName>>("potential");
  }

  params.set<std::vector<VariableName>>("electrons") = {getParam<std::string>("electron_density")};

  if (_use_ad)
  {
    _problem->addKernel(kernel_name,
                        "energy_reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            "_" + std::to_string(reaction_num) + "_" + _name,
                        params);
    _problem->haveADObjects(true);
  }
  else
    _problem->addKernel(kernel_name,
                        "energy_reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            "_" + std::to_string(reaction_num) + "_" + _name,
                        params);
}

void
AddZapdosReactions::addEEDFCoefficient(const unsigned & reaction_num)
{
  std::string material_name;
  if (_coefficient_format == "townsend")
  {
    if (_interpolation_type == "spline")
      material_name = _ad_prepend + "EEDFRateConstantTownsend";
    else if (_interpolation_type == "linear")
      material_name = "TownsendCoefficientLinear";
  }
  else
    material_name = _ad_prepend + "ZapdosEEDFRateConstant";

  auto params = _factory.getValidParams(material_name);
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("file_location") = getParam<std::string>("file_location");
  params.set<std::vector<VariableName>>("electrons") = {
      _reactants[reaction_num][_electron_index[reaction_num]]};
  params.set<std::vector<VariableName>>("mean_energy") = {_electron_energy[0]};
  if (_is_identified[reaction_num])
  {
    params.set<FileName>("property_file") = _reaction_identifier[reaction_num];
  }
  else
  {
    params.set<FileName>("property_file") = "reaction_" + _reaction[reaction_num] + ".txt";
  }

  params.set<std::string>("number") = Moose::stringify(reaction_num);
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  if (_use_ad)
  {
    _problem->addMaterial(material_name,
                          "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                              std::to_string(reaction_num) + "_" + _name,
                          params);
    _problem->haveADObjects(true);
  }
  else
  {
    _problem->addMaterial(material_name,
                          "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                              std::to_string(reaction_num) + "_" + _name,
                          params);
  }
}

void
AddZapdosReactions::addConstantRateCoefficient(const unsigned & reaction_num)
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
AddZapdosReactions::addFunctionRateCoefficient(const unsigned & reaction_num)
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
AddZapdosReactions::addSuperelasticRateCoefficient(const unsigned & reaction_num)
{
  /*
   * THIS IS A WORK IN PROGRESS.
   */
  // first we need to figure out which participants exist, and pass only
  // those stoichiometric coefficients and names.
  std::vector<std::string> active_participants;

  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    active_participants.push_back(_reactants[reaction_num][k]);
  }
  for (unsigned int k = 0; k < _products[reaction_num].size(); ++k)
  {
    active_participants.push_back(_products[reaction_num][k]);
  }
  sort(active_participants.begin(), active_participants.end());
  std::vector<std::string>::iterator it;
  it = std::unique(active_participants.begin(), active_participants.end());
  active_participants.resize(std::distance(active_participants.begin(), it));

  // Now we find the correct index to obtain the necessary stoichiometric values
  std::vector<std::string>::iterator iter;
  std::vector<Real> active_constants;
  for (unsigned int k = 0; k < active_participants.size(); ++k)
  {
    iter = std::find(_all_participants.begin(), _all_participants.end(), active_participants[k]);
    active_constants.push_back(
        _stoichiometric_coeff[reaction_num][std::distance(_all_participants.begin(), iter)]);
  }

  InputParameters params = _factory.getValidParams("SuperelasticReactionRate");
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("original_reaction") = _reaction[_superelastic_index[reaction_num]];
  params.set<std::vector<Real>>("stoichiometric_coeff") = active_constants;
  params.set<std::vector<std::string>>("participants") = active_participants;
  params.set<std::string>("file_location") = "PolynomialCoefficients";
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  params.set<std::string>("number") = Moose::stringify(reaction_num);
  _problem->addMaterial("SuperelasticReactionRate",
                        "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                            std::to_string(reaction_num) + "_" + _name,
                        params);
}

void
AddZapdosReactions::addFunctionKernel(const unsigned & reaction_num,
                                      const unsigned & species_num,
                                      const std::string & kernel_name,
                                      const bool & energy_kernel)
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

  if (energy_kernel)
  {
    params.set<NonlinearVariableName>("variable") = {_electron_energy[0]};
    for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
      params.set<std::vector<VariableName>>(_reactant_names[k]) = {_reactants[reaction_num][k]};
    params.set<Real>("coefficient") = 1;
    params.set<Real>("threshold_energy") = _threshold_energy[reaction_num];
    kernel_identifier = "kernel_function_energy_" +
                        getParam<std::vector<SubdomainName>>("block")[0] +
                        std::to_string(reaction_num) + "_" + std::to_string(species_num);
  }
  else
  {
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
  }

  _problem->addKernel(kernel_name, kernel_identifier + "_" + _name, params);
}

void
AddZapdosReactions::addConstantKernel(const unsigned & reaction_num,
                                      const unsigned & species_num,
                                      const std::string & kernel_name,
                                      const bool & energy_kernel)
{
  std::string kernel_identifier;

  InputParameters params = _factory.getValidParams(kernel_name);
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("number") = Moose::stringify(reaction_num);
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  // TO DO: (1) ALLOW FOR BOTH ELECTRON AND GAS TEMPERATURE

  if (energy_kernel)
  {
    params.set<NonlinearVariableName>("variable") = {_electron_energy[0]};
    for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
      params.set<std::vector<VariableName>>(_reactant_names[k]) = {_reactants[reaction_num][k]};
    params.set<Real>("coefficient") = 1;
    params.set<Real>("threshold_energy") = _threshold_energy[reaction_num];
    kernel_identifier = "kernel_constant_energy_" +
                        getParam<std::vector<SubdomainName>>("block")[0] +
                        std::to_string(reaction_num) + "_" + std::to_string(species_num);
  }
  else
  {
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
  }

  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  _problem->addKernel(kernel_name, kernel_identifier + "_" + _name, params);
}
