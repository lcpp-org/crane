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
