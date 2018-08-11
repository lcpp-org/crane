#include "Reactant2Body.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", Reactant2Body);

template <>
InputParameters
validParams<Reactant2Body>()
{
  InputParameters params = validParams<NodalKernel>();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("rate_coefficient", "The reaction rate coefficient.");
  return params;
}

Reactant2Body::Reactant2Body(const InputParameters & parameters)
  : NodalKernel(parameters),
    _coupled_var_A(*getVar("v", 0)),
    _v(coupledValue("v")),
    _rate_coefficient(getParam<Real>("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
Reactant2Body::computeQpResidual()
{
  return -_stoichiometric_coeff * _rate_coefficient * _u[_qp] * _v[_qp];
}


Real
Reactant2Body::computeQpJacobian()
{
  return -_stoichiometric_coeff * _rate_coefficient * _v[_qp];
}
