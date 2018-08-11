#include "ProductFirstOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductFirstOrder);

template <>
InputParameters
validParams<ProductFirstOrder>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ProductFirstOrder::ProductFirstOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ProductFirstOrder::computeQpResidual()
{
  if (isCoupled("v"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * _v[_qp];
  }
  else
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * getMaterialProperty<Real>("n_gas")[_qp];
  }
}

Real
ProductFirstOrder::computeQpJacobian()
{
  return 0.0;
}

Real
ProductFirstOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isCoupled("v"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * _v[_qp] * _phi[_j][_qp];
  }
  else
  {
    return 0;
  }
}
