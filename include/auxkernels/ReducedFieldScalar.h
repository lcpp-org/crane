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

#include "AuxScalarKernel.h"

/**
 * Explicit solve of ODE:
 *
 * dy/dt = -\lambda y  (using forward Euler)
 */
class ReducedFieldScalar : public AuxScalarKernel
{
public:
  ReducedFieldScalar(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~ReducedFieldScalar();

protected:
  virtual Real computeValue();

  const VariableValue & _electron_density;
  const VariableValue & _gas_density;
};
