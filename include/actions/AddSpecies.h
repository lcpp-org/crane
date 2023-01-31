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

class AddSpecies : public AddVariableAction
{
public:
  AddSpecies(const InputParameters & params);

  static InputParameters validParams();

  virtual void act() override;

  void createInitialConditions(const std::string & var_name, const Real & value);

private:
  /// Primary species to add
  const std::vector<NonlinearVariableName> _vars;
  const std::vector<Real> _vals;
  bool _use_scalar;
  bool _add_time_derivatives;
  bool _use_log;
  /// Variable scaling
  const std::vector<Real> _scale_factor;
};
