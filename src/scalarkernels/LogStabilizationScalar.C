//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LogStabilizationScalar.h"

registerMooseObject("CraneApp", LogStabilizationScalar);

InputParameters
LogStabilizationScalar::validParams()
{
  InputParameters params = ODEKernel::validParams();
  params.addRequiredParam<Real>("offset",
                                "The offset parameter that goes into the exponential function");
  return params;
}

LogStabilizationScalar::LogStabilizationScalar(const InputParameters & parameters)
  : ODEKernel(parameters), _offset(getParam<Real>("offset"))
{
}

LogStabilizationScalar::~LogStabilizationScalar() {}

Real
LogStabilizationScalar::computeQpResidual()
{
  return -std::exp(-(_offset + _u[_i]));
}

Real
LogStabilizationScalar::computeQpJacobian()
{
  return std::exp(-(_offset + _u[_i]));
}
