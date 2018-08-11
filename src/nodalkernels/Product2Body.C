#include "Product2Body.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", Product2Body);

template <>
InputParameters
validParams<Product2Body>()
{
  InputParameters params = validParams<NodalKernel>();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("rate_coefficient", "The reaction rate coefficient.");
  params.declareControllable("coefficient");
  params.declareControllable("rate_coefficient");
  return params;
}

Product2Body::Product2Body(const InputParameters & parameters)
  : NodalKernel(parameters),
    _coupled_var_A(*getVar("v", 0)),
    _v(coupledValue("v")),
    _coupled_var_B(*getVar("w", 0)),
    _w(coupledValue("w")),
    _rate_coefficient(getParam<Real>("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
Product2Body::computeQpResidual()
{
  return -(_stoichiometric_coeff) * _rate_coefficient * _v[_qp] * _w[_qp];
}

Real
Product2Body::computeQpJacobian()
{
  return 0;
}
