//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ODETimeDerivativeTemperature.h"

registerMooseObject("CraneApp", ODETimeDerivativeTemperature);

InputParameters
ODETimeDerivativeTemperature::validParams()
{
  InputParameters params = ODETimeKernel::validParams();
  params.addRequiredParam<Real>("n_gas", "The gas density.");
  return params;
}

ODETimeDerivativeTemperature::ODETimeDerivativeTemperature(const InputParameters & parameters)
 : ODETimeKernel(parameters),
   _n_gas(getParam<Real>("n_gas"))
{
}

Real
ODETimeDerivativeTemperature::computeQpResidual()
{
  return (_n_gas/((5.0/3.0) - 1.0)) * _u_dot[_i];
}

Real
ODETimeDerivativeTemperature::computeQpJacobian()
{
  if (_i == _j)
    return (_n_gas/((5.0/3.0) - 1.0)) * _du_dot_du[_i];
  else
    return 0;
}
