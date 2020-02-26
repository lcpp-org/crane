#include "Zapdos2.h"
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

registerMooseAction("CraneApp", Zapdos2, "add_aux_variable");
registerMooseAction("CraneApp", Zapdos2, "add_aux_kernel");
registerMooseAction("CraneApp", Zapdos2, "add_material");
registerMooseAction("CraneApp", Zapdos2, "add_kernel");
registerMooseAction("CraneApp", Zapdos2, "add_function");

template <>
InputParameters
validParams<Zapdos2>()
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

Zapdos2::Zapdos2(InputParameters params)
  : ChemicalReactionsBase(params),
    _coefficient_format(getParam<std::string>("reaction_coefficient_format")),
    _aux_species(getParam<std::vector<std::string>>("aux_species")),
    _use_ad(getParam<bool>("use_ad"))

{
  if (_coefficient_format == "townsend" && !isParamValid("electron_density"))
    mooseError(
        "Coefficient format type 'townsend' requires an input parameter 'electron_density'!");

  if (_coefficient_format == "townsend" && !isParamValid("electron_energy"))
    mooseError("Coefficient format type 'townsend' requires an input parameter 'electron_energy'!");

  if (_use_ad)
  {
    _ad_prepend = "AD";
    _ad_append = "<RESIDUAL>";
  }
  else
  {
    _ad_prepend = "";
    _ad_append = "";
  }
}

void
Zapdos2::act()
{
  std::vector<int> other_index;
  std::vector<int> reactant_indices;
  std::vector<bool> reactant_species; // This says whether the reactant corresponding to
                                      // reactant_indices is a species.
  // std::vector<bool> reactant_aux; // This says whether each reactant is an aux variable or not.
  // This is important because aux variables do not contribute to the jacobian!
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
  std::vector<NonlinearVariableName> variables =
      getParam<std::vector<NonlinearVariableName>>("species");
  // std::vector<VariableName> electron_energy =
  // getParam<std::vector<VariableName>>("electron_energy");
  // std::string electron_density = getParam<std::string>("electron_density");

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
        if (_track_rates == true)
        {
          auto params = _factory.getValidParams("MooseVariableConstMonomial");
          params.set<MooseEnum>("order") = "CONSTANT";
          params.set<MooseEnum>("family") = "MONOMIAL";
          _problem->addAuxVariable(
              "MooseVariableConstMonomial", "rate" + std::to_string(i), params);
        }
      }
    }
  }

  if (_current_task == "add_aux_kernel")
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {

      if (_track_rates == true)
      {
        if (_coefficient_format == "townsend") // Use townsend rate format
        {
          InputParameters params = _factory.getValidParams("ReactionRateTownsend");
          params.set<std::vector<VariableName>>("mean_en") = {_electron_energy[0]};
          if (_coefficient_format == "townsend")
            params.set<std::vector<VariableName>>("potential") =
                getParam<std::vector<VariableName>>("potential");
          params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
          params.set<Real>("position_units") = _r_units;
          params.set<std::string>("reaction") = _reaction[i];
          params.set<std::string>("reaction_coefficient_name") = {"alpha_" + _reaction[i]};
          params.set<AuxVariableName>("variable") = {"rate" + std::to_string(i)};
          params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
          _problem->addAuxKernel(
              "ReactionRateTownsend", "Calc_Zapdos_Production_Rate" + std::to_string(i), params);
        }
        else
        {
          if (_reactants[i].size() == 1)
          {
            InputParameters params = _factory.getValidParams("ReactionRateOneBody");
            params.set<std::vector<VariableName>>("v") = {(_reactants[i][0])};
            params.set<AuxVariableName>("variable") = {"rate" + std::to_string(i)};
            params.set<std::string>("reaction") = _reaction[i];
            params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
            _problem->addAuxKernel(
                "ReactionRateOneBody", "Calc_Zapdos_Production_Rate" + std::to_string(i), params);
          }
          else if (_reactants[i].size() == 2)
          {
            InputParameters params = _factory.getValidParams("ReactionRateTwoBody");
            params.set<std::vector<VariableName>>("v") = {(_reactants[i][0])};
            params.set<std::vector<VariableName>>("w") = {(_reactants[i][1])};
            params.set<AuxVariableName>("variable") = {"rate" + std::to_string(i)};
            params.set<std::string>("reaction") = _reaction[i];
            params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
            _problem->addAuxKernel(
                "ReactionRateTwoBody", "Calc_Zapdos_Production_Rate" + std::to_string(i), params);
          }

          else if (_reactants[i].size() == 3)
          {
            InputParameters params = _factory.getValidParams("ReactionRateThreeBody");
            params.set<std::vector<VariableName>>("v") = {(_reactants[i][0])};
            params.set<std::vector<VariableName>>("w") = {(_reactants[i][1])};
            params.set<std::vector<VariableName>>("vv") = {(_reactants[i][2])};
            params.set<AuxVariableName>("variable") = {"rate" + std::to_string(i)};
            params.set<std::string>("reaction") = _reaction[i];
            params.set<ExecFlagEnum>("execute_on") = "TIMESTEP_BEGIN";
            _problem->addAuxKernel(
                "ReactionRateThreeBody", "Calc_Zapdos_Production_Rate" + std::to_string(i), params);
          }
        }
      }
    }
  }

  // Add all rate coefficients as materials
  if (_current_task == "add_material")
  {
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
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
        addFunctionRateCoefficient(i);
      }
      else if (_superelastic_reaction[i] == true)
      {
        mooseError("Superelastic reactions not supported yet!");
        addSuperelasticRateCoefficient(i);
      }
      // Now we check for reactions that include a change of energy.
      // Will this require  its own material?
      // if (_energy_change[i] == true)
      // {
      // Gas temperature is almost in place, but not finished yet.
      // std::cout << "WARNING: energy dependence is not yet implemented." << std::endl;
      // }
    }
  }

  // Add appropriate kernels to each reactant and product.
  if (_current_task == "add_kernel")
  {
    int non_electron_index;
    int index; // stores index of species in the reactant/product arrays
    std::vector<std::string>::iterator iter;
    std::vector<std::string>::iterator iter_aux;
    for (unsigned int i = 0; i < _num_reactions; ++i)
    {
      energy_kernel_name = "ADElectronEnergyTerm";
      if (_elastic_collision[i])
        energy_kernel_name += "Elastic";
      // if (!isnan(_rate_coefficient[i]) || _rate_equation[i] == true || _superelastic_reaction[i]
      // == true || getParam<bool>("track_electron_energy") == false)
      if (_coefficient_format == "townsend" && _rate_type[i] == "EEDF")
      {
        energy_kernel_name += "Townsend";

        if (_use_ad)
        {
          product_kernel_name = "ADEEDFRxnProd";
          reactant_kernel_name = "ADEEDFRxnReac";
        }
        else
        {
          product_kernel_name = "ElectronImpactReactionProduct";
          reactant_kernel_name = "ElectronImpactReactionReactant";
        }
      }
      // else if (_coefficient_format == "rate")

      else if (_coefficient_format == "rate" && _rate_type[i] == "EEDF")
      {
        energy_kernel_name += "Rate";
        // energy_kernel_name = "EnergyTermRate";
        if (_reactants[i].size() == 1)
        {
          product_kernel_name = "ProductFirstOrder";
          reactant_kernel_name = "ReactantFirstOrder";
        }
        else if (_reactants[i].size() == 2)
        {
          product_kernel_name = "ElectronProductSecondOrder";
          reactant_kernel_name = "ElectronReactantSecondOrder";
        }
        else
        {
          product_kernel_name = "ProductThirdOrder";
          reactant_kernel_name = "ReactantThirdOrder";
        }
        if (_use_log)
        {
          product_kernel_name += "Log";
          reactant_kernel_name += "Log";
        }
      }
      else
      {
        // energy_kernel_name += "Rate";
        energy_kernel_name = "EnergyTermRate";
        if (_reactants[i].size() == 1)
        {
          product_kernel_name = "ProductFirstOrder";
          reactant_kernel_name = "ReactantFirstOrder";
        }
        else if (_reactants[i].size() == 2)
        {
          product_kernel_name = "ProductSecondOrder";
          reactant_kernel_name = "ReactantSecondOrder";
        }
        else
        {
          product_kernel_name = "ProductThirdOrder";
          reactant_kernel_name = "ReactantThirdOrder";
        }
        if (_use_log)
        {
          product_kernel_name += "Log";
          reactant_kernel_name += "Log";
        }
      }
      // std::cout << energy_kernel_name << ", " << _energy_change[i] << std::endl;

      // if (_energy_change[i] && _rate_type[i] == "EEDF")
      // {
      if (_energy_change[i])
      {
        Real energy_sign;
        for (unsigned int t = 0; t < _energy_variable.size(); ++t)
        {
          if (_electron_energy_term[t])
            energy_sign = 1.0;
          else
            energy_sign = -1.0;

          if (_rate_type[i] == "EEDF")
          {
            addEEDFEnergy(i);
          }
          else if (_rate_type[i] != "EEDF")
          {
            addRateEnergy(i);
          }
        }
      }

      for (MooseIndex(_species) j = 0; j < _species.size(); ++j)
      {
        iter = std::find(_reactants[i].begin(), _reactants[i].end(), _species[j]);
        index = std::distance(_reactants[i].begin(), iter);
        /*
        if (iter == _reactants[i].end())
        {
          continue;
        }
        */

        // Here we check to see if the current species is an auxiliary variable.
        // If it is, no kernels are added and we skip to the next species.
        iter_aux = std::find(_aux_species.begin(), _aux_species.end(), _species[j]);
        if (iter_aux != _aux_species.end())
          continue;

        if (iter != _reactants[i].end())
        {
          // Now we see if the second reactant is a tracked species.
          // We only treat two-body reactions now. This will need to be changed for three-body
          // reactions. e.g. 1) find size of reactants array 2) use find() to search other values
          // inside that size that are not == index 3) same result!
          reactant_indices.resize(_reactants[i].size());
          for (unsigned int k = 0; k < _reactants[i].size(); ++k)
            reactant_indices[k] = k;
          reactant_indices.erase(reactant_indices.begin() + index);
          reactant_species.resize(reactant_indices.size());
          for (unsigned int k = 0; k < reactant_indices.size(); ++k)
          {
            find_other =
                std::find(_species.begin(), _species.end(), _reactants[i][reactant_indices[k]]) !=
                _species.end();
            if (!find_other)
              find_other = std::find(_aux_species.begin(),
                                     _aux_species.end(),
                                     _reactants[i][reactant_indices[k]]) != _aux_species.end();
            if (find_other)
              continue;
            else
              reactant_indices.erase(reactant_indices.begin() + k);
          }
          /*v_index = std::abs(index - 1);*/
          /*find_other =*/
          /*std::find(_species.begin(), _species.end(), _reactants[i][v_index]) != _species.end();*/
          /*find_aux = std::find(_aux_species.begin(), _aux_species.end(), _reactants[i][v_index])
           * !=*/
          /*_aux_species.end();*/
          if (_species_count[i][j] < 0)
          {
            if (_coefficient_format == "townsend" && _rate_type[i] == "EEDF")
            {
              addEEDFKernel(i, j, reactant_kernel_name);
              /*
              InputParameters params = _factory.getADValidParams(reactant_kernel_name);
              // params.set<NonlinearVariableName>("variable") = _reactants[i][index];
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<std::vector<VariableName>>("mean_en") = {_electron_energy[0]};
              params.set<std::vector<VariableName>>("potential") =
                  getParam<std::vector<VariableName>>("potential");
              params.set<std::vector<VariableName>>("em") = {
                  getParam<std::string>("electron_density")};
              params.set<Real>("position_units") = _r_units;
              params.set<std::string>("reaction") = _reaction[i];
              params.set<std::string>("reaction_coefficient_name") = _reaction_coefficient_name[i];
              params.set<std::vector<SubdomainName>>("block") =
                  getParam<std::vector<SubdomainName>>("block");
              if (find_other)
              {
                //                  std::cout << _reaction[i] << std::endl;
                params.set<std::vector<VariableName>>("target") = {
                    _reactants[i][non_electron_index]};
              }
              _problem->addADKernel(reactant_kernel_name,
                                    "kernel" + std::to_string(i) + "_" + std::to_string(j),
                                    params);
              */
            }
            // else if (_coefficient_format == "rate" || _rate_type[i] != "EEDF")
            else if (_coefficient_format == "rate" && _rate_type[i] == "EEDF")
            {
              /*InputParameters params = _factory.getValidParams("Electron" +
               * reactant_kernel_name);*/
              addRateReactantKernel(i, j, reactant_kernel_name, reactant_indices, other_variables);
              /*
              InputParameters params = _factory.getValidParams(reactant_kernel_name);
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<Real>("coefficient") = _species_count[i][j];
              params.set<std::string>("reaction") = _reaction[i];
              if (isParamValid("electron_energy"))
                params.set<std::vector<VariableName>>("energy") = {_electron_energy[0]};
              if (find_other || find_aux)
              {
                for (unsigned int k = 0; k < reactant_indices.size(); ++k)
                {
                  params.set<std::vector<VariableName>>(other_variables[k]) = {
                      _reactants[i][reactant_indices[k]]};
                  // If the current species does not equal the electron density, then BY
                  // DEFINITION the other species must be the electron density.
                  //if (_species[j] != getParam<std::string>("electron_density"))
                  //params.set<bool>("_" + other_variables[k] + "_eq_electron") = true;
                }
              }
              params.set<std::vector<SubdomainName>>("block") =
                  getParam<std::vector<SubdomainName>>("block");

              //_problem->addKernel("Electron" + reactant_kernel_name,
              _problem->addKernel(reactant_kernel_name,
                                  "kernel" + std::to_string(i) + "_" + std::to_string(j),
                                  params);
              */
            }
            else
            {
              addRateReactantKernel(i, j, reactant_kernel_name, reactant_indices, other_variables);

              /*
              InputParameters params = _factory.getValidParams(reactant_kernel_name);
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<Real>("coefficient") = _species_count[i][j];
              // std::cout << getParam<std::vector<SubdomainName>>("block")[0] << ", " <<
              // _species_count[i][j] << std::endl; mooseError("TESTING");
              params.set<std::string>("reaction") = _reaction[i];
              if (find_other || find_aux)
              {
                for (unsigned int k = 0; k < reactant_indices.size(); ++k)
                {
                  params.set<std::vector<VariableName>>(other_variables[k]) = {
                      _reactants[i][reactant_indices[k]]};
                  if (_species[j] == _reactants[i][reactant_indices[k]])
                    params.set<bool>("_" + other_variables[k] + "_eq_u") = true;
                }
                // params.set<std::vector<VariableName>>("v") = {_reactants[i][v_index]};
              }
              params.set<std::vector<SubdomainName>>("block") =
                  getParam<std::vector<SubdomainName>>("block");

              _problem->addKernel(reactant_kernel_name,
                                  "kernel" + std::to_string(i) + "_" + std::to_string(j),
                                  params);
              */
            }
          }
        }

        // Now we do the same thing for the products side of the reaction
        iter = std::find(_products[i].begin(), _products[i].end(), _species[j]);
        // index = std::distance(_products[i].begin(), iter);
        // species_v = std::find(_species.begin(), _species.end(), _reactants[i][0]) !=
        // _species.end(); species_w = std::find(_species.begin(), _species.end(), _reactants[i][1])
        // != _species.end();
        include_species.resize(_reactants[i].size());
        for (unsigned int k = 0; k < _reactants[i].size(); ++k)
        {
          include_species[k] =
              std::find(_species.begin(), _species.end(), _reactants[i][k]) != _species.end();
          // if (std::find(_aux_species.begin(), _aux_species.end(), _reactants[i][k]) !=
          // _aux_species.end()) include_species[k] = false;
          if (include_species[k] == false)
            include_species[k] =
                std::find(_aux_species.begin(), _aux_species.end(), _reactants[i][k]) !=
                _aux_species.end();
        }
        if (iter != _products[i].end())
        {

          if (_species_count[i][j] > 0)
          {
            if (_coefficient_format == "townsend" && _rate_type[i] == "EEDF")
            {
              addEEDFKernel(i, j, product_kernel_name);
            }
            // else if (_coefficient_format == "rate")
            else if (_coefficient_format == "rate" && _rate_type[i] == "EEDF")
            {
              /*InputParameters params = _factory.getValidParams("Electron" +
               * product_kernel_name);*/
              //addRateProductKernel(i, j, product_kernel_name, reactant_indices, other_variables);
              
              InputParameters params = _factory.getValidParams(product_kernel_name);
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<std::string>("reaction") = _reaction[i];
              params.set<Real>("coefficient") = _species_count[i][j];
              params.set<std::vector<VariableName>>("electron") = {
                  getParam<std::string>("electron_density")};

              if (isParamValid("electron_energy"))
              {
                params.set<std::vector<VariableName>>("energy") = {_electron_energy[0]};
                // getParam<std::vector<VariableName>>("electron_energy");
              }

              std::string other;
              for (unsigned int k = 0; k < _reactants[i].size(); ++k)
              {
                if (k == 0)
                  other = "_v_eq_u";
                else if (k == 1)
                  other = "_w_eq_u";
                if (_reactants[i][k] == getParam<std::string>("electron_density"))
                {
                  // params.set<std::vector<VariableName>>("electron") = {_electron_energy[0]};
                  if (_species[j] == _reactants[i][k])
                    params.set<bool>("_electron_eq_u") = true;
                }
                // else if (include_species[k])
                else if (include_species[k] &&
                         _reactants[i][k] != getParam<std::string>("electron_density"))
                {
                  // params.set<std::vector<VariableName>>(other_variables[k]) = {_reactants[i][k]};
                  params.set<std::vector<VariableName>>("target") = {_reactants[i][k]};
                  // This next if statement should never be satisfied...
                  if (_species[j] == _reactants[i][k])
                    params.set<bool>(other) = true;
                  //                    params.set<bool>("_target_eq_u") = true;
                }
              }
              params.set<std::vector<SubdomainName>>("block") =
                  getParam<std::vector<SubdomainName>>("block");
              //_problem->addKernel("Electron" + product_kernel_name,
              _problem->addKernel(product_kernel_name,
                                  "kernel_prod" + std::to_string(j) + "_" + _reaction[i],
                                  params);
            }
            else
            {
              InputParameters params = _factory.getValidParams(product_kernel_name);
              params.set<NonlinearVariableName>("variable") = _species[j];
              params.set<std::string>("reaction") = _reaction[i];
              // This loop includes reactants as long as they are tracked species.
              // If a species is not tracked, it is treated as a background gas.
              for (unsigned int k = 0; k < _reactants[i].size(); ++k)
              {
                // std::cout << _reaction[i] << ": \n" << "  " << _reactants[i][k] << ", " <<
                // include_species[k] << std::endl;
                if (include_species[k])
                {
                  params.set<std::vector<VariableName>>(other_variables[k]) = {_reactants[i][k]};
                  if (_species[j] == _reactants[i][k])
                  {
                    params.set<bool>("_" + other_variables[k] + "_eq_u") = true;
                  }
                }
              }
              params.set<Real>("coefficient") = _species_count[i][j];
              params.set<std::vector<SubdomainName>>("block") =
                  getParam<std::vector<SubdomainName>>("block");
              _problem->addKernel(product_kernel_name,
                                  "kernel_prod" + std::to_string(j) + "_" + _reaction[i],
                                  params);
            }
          }
        }
      }

      // To do: add energy kernels here
    }
  }
}

void
Zapdos2::addEEDFKernel(const unsigned & reaction_num,
                       const unsigned & species_num,
                       const std::string & kernel_name)
{
  auto params = _factory.getValidParams(kernel_name + _ad_append);
  params.set<NonlinearVariableName>("variable") = _species[species_num];
  params.set<std::vector<VariableName>>("mean_en") = {_electron_energy[0]};

  params.set<std::vector<VariableName>>("em") = {getParam<std::string>("electron_density")};
  params.set<Real>("position_units") = _r_units;
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("reaction_coefficient_name") = _reaction_coefficient_name[reaction_num];
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  // For townsend coefficients, potential variable is needed to compute electron flux
  if (_coefficient_format == "townsend")
    params.set<std::vector<VariableName>>("potential") =
        getParam<std::vector<VariableName>>("potential");

  if (_use_ad)
  {
    _problem->addKernel(kernel_name + "<RESIDUAL>",
                        "kernel_eedf_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            std::to_string(reaction_num) + std::to_string(species_num) +
                            "_residual",
                        params);
    _problem->addKernel(kernel_name + "<JACOBIAN>",
                        "kernel_eedf_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            std::to_string(reaction_num) + std::to_string(species_num) +
                            "_jacobian",
                        params);
    _problem->haveADObjects(true);
  }
  else
  {
    /*
    if (find_other)
    {
      params.set<std::vector<VariableName>>("target") = {_reactants[i][non_electron_index]};
    }
    */

    _problem->addKernel(kernel_name,
                        "kernel_eedf_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            std::to_string(reaction_num) + std::to_string(species_num),
                        params);
  }
}

void
Zapdos2::addEEDFCoefficient(const unsigned & reaction_num)
{
  std::string material_name = _ad_prepend + "EEDFRateConstant";

  if (_coefficient_format == "townsend")
  {
    material_name += "Townsend";
  }
  auto params = _factory.getValidParams(material_name + _ad_append);
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<std::string>("file_location") = getParam<std::string>("file_location");
  params.set<Real>("position_units") = getParam<Real>("position_units");
  params.set<std::vector<VariableName>>("em") = {
      _reactants[reaction_num][_electron_index[reaction_num]]};
  params.set<std::vector<VariableName>>("mean_en") = {_electron_energy[0]};

  unsigned int target; // stores index of target species for electron-impact reactions
  // InputParameters params = _factory.getValidParams("ADEEDFRateConstantTownsend<RESIDUAL>");

  bool target_species_tracked = false;
  bool target_species_aux = false;
  for (unsigned int j = 0; j < _species.size(); ++j)
  {
    if (_species[j] == getParam<std::string>("electron_density"))
    {
      continue;
    }

    for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
    {
      if (_reactants[reaction_num][k] == _species[j])
      {
        target_species_tracked = true;
        target = k;
        break;
      }
    }

    if (target_species_tracked)
      break;
  }
  for (unsigned int j = 0; j < _aux_species.size(); ++j)
  {
    for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
    {
      if (_reactants[reaction_num][k] == _aux_species[j])
      {
        target_species_aux = true;
        break;
      }
    }

    if (target_species_aux)
      break;
  }
  if (target_species_tracked)
  {
    params.set<std::vector<VariableName>>("target_species") = {_reactants[reaction_num][target]};
  }
  params.set<bool>("is_target_aux") = target_species_aux;
  params.set<bool>("elastic_collision") = {_elastic_collision[reaction_num]};
  params.set<FileName>("property_file") = "reaction_" + _reaction[reaction_num] + ".txt";
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  if (_use_ad)
  {
    _problem->addADResidualMaterial(material_name + "<RESIDUAL>",
                                    "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                                        "_" + std::to_string(reaction_num) + "_residual",
                                    params);
    _problem->addADJacobianMaterial(material_name + "<JACOBIAN>",
                                    "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                                        "_" + std::to_string(reaction_num) + "_jacobian",
                                    params);
    _problem->haveADObjects(true);
  }
  else
  {
    _problem->addMaterial(material_name,
                          "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                              std::to_string(reaction_num),
                          params);
  }
}

void
Zapdos2::addConstantRateCoefficient(const unsigned & reaction_num)
{
  InputParameters params = _factory.getValidParams("GenericRateConstant");
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<Real>("reaction_rate_value") = _rate_coefficient[reaction_num];
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  _problem->addMaterial("GenericRateConstant",
                        "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                            std::to_string(reaction_num),
                        params);
}

void
Zapdos2::addFunctionRateCoefficient(const unsigned & reaction_num)
{
  InputParameters params = _factory.getValidParams("DerivativeParsedMaterial");
  params.set<std::string>("f_name") = "k_" + _reaction[reaction_num];
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
                            std::to_string(reaction_num),
                        params);
}

void
Zapdos2::addSuperelasticRateCoefficient(const unsigned & reaction_num)
{

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
  _problem->addMaterial("SuperelasticReactionRate",
                        "reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                            std::to_string(reaction_num),
                        params);
}

// Add energy term from a reaction that uses a rate coefficient computed by EEDF convolution
void
Zapdos2::addEEDFEnergy(const unsigned & reaction_num)
{
  std::string kernel_name = _ad_prepend + "ElectronEnergyTerm";
  unsigned int non_electron_index;
  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    if (_reactants[reaction_num][k] == "em")
      continue;
    else
      non_electron_index = k;
  }

  if (_elastic_collision[reaction_num])
  {
    kernel_name += "Elastic";
  }

  if (_coefficient_format == "townsend")
    kernel_name += "Townsend";
  InputParameters params = _factory.getValidParams(kernel_name + _ad_append);

  // If this is not an elastic collision, the change in enthalpy needs to be included
  if (!_elastic_collision[reaction_num])
    params.set<Real>("threshold_energy") = _threshold_energy[reaction_num];

  params.set<NonlinearVariableName>("variable") = {_electron_energy[0]};
  params.set<std::vector<VariableName>>("target_species") = {
      _reactants[reaction_num][non_electron_index]};
  // params.set<Real>("threshold_energy") = energy_sign * _threshold_energy[reaction_num];

  params.set<std::vector<VariableName>>("target_species") = {
      _reactants[reaction_num][non_electron_index]};
  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<Real>("position_units") = _r_units;
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  if (_coefficient_format == "townsend")
    params.set<std::vector<VariableName>>("potential") =
        getParam<std::vector<VariableName>>("potential");
  else
    params.set<std::vector<VariableName>>("v") = {_reactants[reaction_num][non_electron_index]};

  params.set<std::vector<VariableName>>("electron_species") = {
      getParam<std::string>("electron_density")};

  if (_use_ad)
  {
    _problem->addKernel(kernel_name + "<RESIDUAL>",
                        "energy_reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            "_" + std::to_string(reaction_num) + "_residual",
                        params);
    _problem->addKernel(kernel_name + "<JACOBIAN>",
                        "energy_reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            "_" + std::to_string(reaction_num) + "_jacobian",
                        params);
    _problem->haveADObjects(true);
  }
  else
    _problem->addKernel(kernel_name,
                        "energy_reaction_" + getParam<std::vector<SubdomainName>>("block")[0] +
                            "_" + std::to_string(reaction_num),
                        params);
}

void
Zapdos2::addRateEnergy(const unsigned & reaction_num)
{
  ////////////////
  // NEEDS AD IMPLEMENTATION
  // ////////////
  unsigned int non_electron_index;
  std::string kernel_name = "EnergyTermRate";
  InputParameters params = _factory.getValidParams(kernel_name);
  params.set<NonlinearVariableName>("variable") = {_electron_energy[0]};
  params.set<std::vector<VariableName>>("electron_species") = {
      getParam<std::string>("electron_density")};
  // Find the non-electron reactant
  for (unsigned int k = 0; k < _reactants[reaction_num].size(); ++k)
  {
    if (_reactants[reaction_num][k] == "em")
      continue;
    else
      non_electron_index = k;
  }

  // Other species should ALWAYS be tracked
  params.set<std::vector<VariableName>>("v") = {_reactants[reaction_num][non_electron_index]};

  params.set<std::string>("reaction") = _reaction[reaction_num];
  params.set<Real>("threshold_energy") = _threshold_energy[reaction_num];
  params.set<Real>("position_units") = _r_units;
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");
  _problem->addKernel(kernel_name,
                      "energy_reaction_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" +
                          std::to_string(reaction_num),
                      params);
}

void
Zapdos2::addRateReactantKernel(const unsigned & reaction_num,
                       const unsigned & species_num,
                       const std::string & kernel_name,
                       const std::vector<int> & indices,
                       const std::vector<std::string> & variables)
{
  ////////////////
  // NEEDS AD IMPLEMENTATION
  // ////////////
  InputParameters params = _factory.getValidParams(kernel_name + _ad_append);
  params.set<NonlinearVariableName>("variable") = _species[species_num];
  params.set<Real>("coefficient") = _species_count[reaction_num][species_num];
  params.set<std::string>("reaction") = _reaction[reaction_num];
  if (isParamValid("electron_energy") && _rate_type[reaction_num] == "EEDF")
    params.set<std::vector<VariableName>>("energy") = {_electron_energy[0]};
  for (unsigned int k = 0; k < indices.size(); ++k)
  {
    params.set<std::vector<VariableName>>(variables[k]) = {
        _reactants[reaction_num][indices[k]]};
  }
  params.set<std::vector<SubdomainName>>("block") = getParam<std::vector<SubdomainName>>("block");

  //_problem->addKernel("Electron" + reactant_kernel_name,
  _problem->addKernel(
      kernel_name, "kernel_" + getParam<std::vector<SubdomainName>>("block")[0] + "_" + std::to_string(reaction_num), params);
}
