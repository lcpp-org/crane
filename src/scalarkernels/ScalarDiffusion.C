//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ScalarDiffusion.h"

registerMooseObject("CraneApp", ScalarDiffusion);

InputParameters
ScalarDiffusion::validParams()
{
  InputParameters params = ODEKernel::validParams();
  params.addRequiredParam<Real>("rate", "The rate of diffusion.");
  return params;
}

ScalarDiffusion::ScalarDiffusion(const InputParameters & parameters)
  : ODEKernel(parameters), _rate(getParam<Real>("rate"))
{
}

Real
ScalarDiffusion::computeQpResidual()
{
  return _rate * _u[_i];
}

Real
ScalarDiffusion::computeQpJacobian()
{
  return _rate;
}

Real
ScalarDiffusion::computeQpOffDiagJacobian(unsigned int)
{
  return 0;
}
