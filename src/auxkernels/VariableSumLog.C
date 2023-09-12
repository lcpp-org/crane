//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "VariableSumLog.h"

registerMooseObject("CraneApp", VariableSumLog);

InputParameters
VariableSumLog::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addCoupledVar("args", "The variables to sum.");
  return params;
}

VariableSumLog::VariableSumLog(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _nargs(coupledScalarComponents("args")),
    _args(_nargs)
{
}

Real
VariableSumLog::computeValue()
{
  Real variable_sum = 0;
  for (unsigned int i = 0; i < _nargs; ++i)
  {
    variable_sum += std::exp(coupledScalarValue("args", i)[_i]);
  }
  return std::log(variable_sum);
}
