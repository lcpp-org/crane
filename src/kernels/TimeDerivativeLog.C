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
