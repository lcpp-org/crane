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

#include "ODEKernel.h"
// #include "RateCoefficientProvider.h"

class Reactant1BodyScalar : public ODEKernel
{
public:
  Reactant1BodyScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  //virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  const VariableValue & _rate_coefficient;
  Real _stoichiometric_coeff;
  bool _rate_constant_equation;

  // const RateCoefficientProvider & _data;
};
