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
  //virtual void addEnergyKernel();
  virtual void addEEDFKernel();
  //virtual void add

  std::string _coefficient_format;
  std::vector<std::string> _aux_species;
  bool _use_ad;
};
