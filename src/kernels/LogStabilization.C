//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "LogStabilization.h"

registerMooseObject("CraneApp", LogStabilization);

InputParameters
LogStabilization::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredParam<Real>("offset",
                                "The offset parameter that goes into the exponential function");
  return params;
}

LogStabilization::LogStabilization(const InputParameters & parameters)
  : Kernel(parameters), _offset(getParam<Real>("offset"))
{
}

LogStabilization::~LogStabilization() {}

Real
LogStabilization::computeQpResidual()
{
  return -_test[_i][_qp] * std::exp(-(_offset + _u[_qp]));
}

Real
LogStabilization::computeQpJacobian()
{
  return -_test[_i][_qp] * std::exp(-(_offset + _u[_qp])) * -_phi[_j][_qp];
}
