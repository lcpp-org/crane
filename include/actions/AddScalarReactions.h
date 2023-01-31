//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
