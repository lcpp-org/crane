//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "VariableSum.h"

registerMooseObject("CraneApp", VariableSum);

InputParameters
VariableSum::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addCoupledVar("args", "The variables to sum.");
  return params;
}

VariableSum::VariableSum(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _nargs(coupledScalarComponents("args")),
    _args(_nargs)
{
}

Real
VariableSum::computeValue()
{
  Real variable_sum = 0;
  for (unsigned int i = 0; i < _nargs; ++i)
  {
    // variables += (i == 0 ? "" : ",") + getScalarVar("args", i)->name();
    // _args[i] = &coupledScalarValue("args", i);
    // std::cout << (*_args[i])[_i] << std::endl;
    variable_sum += coupledScalarValue("args", i)[_i];
  }
  return variable_sum;
}
