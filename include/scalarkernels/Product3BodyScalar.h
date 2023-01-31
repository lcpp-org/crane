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

class Product3BodyScalar : public ODEKernel
{
public:
  Product3BodyScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  unsigned int _v_var;
  const VariableValue & _v;
  unsigned int _w_var;
  const VariableValue & _w;
  unsigned int _x_var;
  const VariableValue & _x;
  const VariableValue & _rate_coefficient;

  Real _stoichiometric_coeff;
  bool _v_eq_u;
  bool _w_eq_u;
  bool _x_eq_u;
  bool _rate_constant_equation;

  // const RateCoefficientProvider & _data;
};
