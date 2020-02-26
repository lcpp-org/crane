#pragma once

#include "AddVariableAction.h"
#include "Action.h"
#include "ChemicalReactionsBase.h"

class Zapdos2;

template <>
InputParameters validParams<Zapdos2>();

class Zapdos2 : public ChemicalReactionsBase
{
public:
  Zapdos2(InputParameters params);

  virtual void act();

protected:
  // virtual void addEnergyKernel();
  virtual void addEEDFKernel(const unsigned & reaction_num,
                             const unsigned & species_num,
                             const std::string & kernel_name);
  virtual void addEEDFCoefficient(const unsigned & reaction_num);
  virtual void addConstantRateCoefficient(const unsigned & reaction_num);
  virtual void addFunctionRateCoefficient(const unsigned & reaction_num);
  virtual void addSuperelasticRateCoefficient(const unsigned & reaction_num);
  virtual void addEEDFEnergy(const unsigned & reaction_num);
  virtual void addRateEnergy(const unsigned & reaction_num);
  virtual void addRateReactantKernel(const unsigned & reaction_num,
                             const unsigned & species_num,
                             const std::string & kernel_name,
                             const std::vector<int> & indices,
                             const std::vector<std::string> & variables);
  // virtual void add

  std::string _coefficient_format;
  std::vector<std::string> _aux_species;
  bool _use_ad;

  std::string _ad_prepend;
  std::string _ad_append;
};
