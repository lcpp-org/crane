//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ODETimeDerivativeLog.h"

registerMooseObject("CraneApp", ODETimeDerivativeLog);

InputParameters
ODETimeDerivativeLog::validParams()
{
  InputParameters params = ODETimeKernel::validParams();
  params.addParam<bool>("lumping", false, "True for mass matrix lumping, false otherwise");
  return params;
}

ODETimeDerivativeLog::ODETimeDerivativeLog(const InputParameters & parameters)
  : ODETimeKernel(parameters), _lumping(getParam<bool>("lumping"))

{
}

Real
ODETimeDerivativeLog::computeQpResidual()
{
  return std::exp(_u[_i]) * _u_dot[_i];
}

Real
ODETimeDerivativeLog::computeQpJacobian()
{
  if (_i == _j)
  {
    return (std::exp(_u[_i]) * _u_dot[_i] +
            std::exp(_u[_i]) * _du_dot_du[_i]);
  }
  else
    return 0;
}
