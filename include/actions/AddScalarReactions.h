#pragma once

#include "AddVariableAction.h"
#include "Action.h"
#include "ChemicalReactionsBase.h"

// class ChemicalReactions : public AddVariableAction
class AddScalarReactions : public ChemicalReactionsBase
{
public:
  AddScalarReactions(const InputParameters & params);
  static InputParameters validParams();

  const std::string _interpolation_type;
  // AddScalarReactions(const InputParameters & params) : ChemicalReactionsBase(params) {};

  virtual void act();

//protected:
  std::vector<std::string> _aux_scalar_var_name;
};
