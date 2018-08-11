#include "ReactantARxn.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactantARxn);

template <>
InputParameters
validParams<ReactantARxn>()
{
  InputParameters params = validParams<NodalKernel>();
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("rate_coefficient", "The reaction rate coefficient.");
  return params;
}

ReactantARxn::ReactantARxn(const InputParameters & parameters)
  : NodalKernel(parameters),
    _rate_coefficient(getParam<Real>("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactantARxn::computeQpResidual()
{
  return -_stoichiometric_coeff * _rate_coefficient * _u[_qp];
}


Real
ReactantARxn::computeQpJacobian()
{
  return -_stoichiometric_coeff * _rate_coefficient;
}
