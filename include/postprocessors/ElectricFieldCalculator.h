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

// MOOSE includes
#include "GeneralPostprocessor.h"

/**
 * This postprocessor computes a side integral of the mass flux.
 */
class ElectricFieldCalculator : public GeneralPostprocessor
{
public:
  ElectricFieldCalculator(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void initialize() override {};
  virtual void execute() override {};
  virtual Real getValue() override;

protected:
  // virtual Real reducedField();

  const VariableValue & _gas_density;
  const VariableValue & _electron_density;

};
