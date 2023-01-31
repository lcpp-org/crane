//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TimeDerivativeLog.h"

registerMooseObject("CraneApp", TimeDerivativeLog);

InputParameters
TimeDerivativeLog::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  return params;
}

TimeDerivativeLog::TimeDerivativeLog(const InputParameters & parameters)
  : ADTimeDerivative(parameters)
{
}

ADReal
TimeDerivativeLog::precomputeQpResidual()
{
  return std::exp(_u[_qp]) * ADTimeDerivative::precomputeQpResidual();
}
