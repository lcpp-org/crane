#pragma once

#include "AddVariableAction.h"
#include "Action.h"
#include "ChemicalReactionsBase.h"

class AddZapdosReactions;

template <>
InputParameters validParams<AddZapdosReactions>();

class AddZapdosReactions : public ChemicalReactionsBase
{
public:
  AddZapdosReactions(InputParameters params);

  virtual void act();

protected:
  // virtual void addEnergyKernel();
  /*
  virtual void addEEDFKernel(const unsigned & reaction_num,
                             const unsigned & species_num,
                             const std::string & kernel_name);
                             */
  virtual void addEEDFCoefficient(const unsigned & reaction_num);
  virtual void addEEDFKernel(const unsigned & reaction_num,
                             const unsigned & species_num,
                             const std::string & kernel_name,
                             const int & electron_index,
                             const int & target_index);
  virtual void addEEDFEnergy(const unsigned & reaction_num, const std::string & kernel_name);
  virtual void addConstantRateCoefficient(const unsigned & reaction_num);
  virtual void addFunctionRateCoefficient(const unsigned & reaction_num);
  virtual void addSuperelasticRateCoefficient(const unsigned & reaction_num);
  virtual void addFunctionKernel(const unsigned & reaction_num,
                                 const unsigned & species_num,
                                 const std::string & kernel_name,
                                 const bool & energy_kernel);
  virtual void addConstantKernel(const unsigned & reaction_num,
                                 const unsigned & species_num,
                                 const std::string & kernel_name,
                                 const bool & energy_kernel);
  virtual std::string
  getKernelName(const unsigned & num_reactants, const bool & energy_kernel, const bool & is_aux);
  virtual std::string getElectronImpactKernelName(const bool & energy_kernel,
                                                  const bool & elastic_kernel,
                                                  const bool & is_aux);

  virtual void addAuxRate(const std::string & aux_kernel_name,
                          const unsigned & reaction_num,
                          const bool & is_townsend);
  /*
  virtual void addEEDFEnergy(const unsigned & reaction_num);
  virtual void addRateEnergy(const unsigned & reaction_num);
  virtual void addRateReactantKernel(const unsigned & reaction_num,
                             const unsigned & species_num,
                             const std::string & kernel_name,
                             const std::vector<int> & indices,
                             const std::vector<std::string> & variables);
  virtual void addRateProductKernel(const unsigned & reaction_num,
                             const unsigned & species_num,
                             const std::string & kernel_name,
                             const std::vector<int> & indices,
                             const std::vector<std::string> & variables);
                             */
  // virtual void add

  std::string _coefficient_format;
  bool _use_ad;

  std::string _ad_prepend;
  std::string _townsend_append;
  std::string _log_append;
  std::vector<std::string> _reactant_names;
};
