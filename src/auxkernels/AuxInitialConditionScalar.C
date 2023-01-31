//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AuxInitialConditionScalar.h"

registerMooseObject("CraneApp", AuxInitialConditionScalar);

InputParameters
AuxInitialConditionScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addParam<Real>("initial_condition", 0, "The initial value of the auxiliary variable.");
  return params;
}

AuxInitialConditionScalar::AuxInitialConditionScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _value(getParam<Real>("initial_condition"))
{
}

Real
AuxInitialConditionScalar::computeValue()
{
  return _value;
}
