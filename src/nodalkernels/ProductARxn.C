#include "ProductARxn.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductARxn);

template <>
InputParameters
validParams<ProductARxn>()
{
  InputParameters params = validParams<NodalKernel>();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("rate_coefficient", "The reaction rate coefficient.");
  params.declareControllable("coefficient");
  params.declareControllable("rate_coefficient");
  return params;
}

ProductARxn::ProductARxn(const InputParameters & parameters)
  : NodalKernel(parameters),
    _coupled_var_A(*getVar("v", 0)),
    _v(coupledValue("v")),
    _rate_coefficient(getParam<Real>("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ProductARxn::computeQpResidual()
{
  return -(_stoichiometric_coeff) * _rate_coefficient * _v[_qp];
}

Real
ProductARxn::computeQpJacobian()
{
  return 0;
}
