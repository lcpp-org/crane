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
