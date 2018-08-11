#include "ReactantSecondOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactantSecondOrder);

template <>
InputParameters
validParams<ReactantSecondOrder>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ReactantSecondOrder::ReactantSecondOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _reaction_coeff(getMaterialProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    // _n_gas(getMaterialProperty<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactantSecondOrder::computeQpResidual()
{
  if (isCoupled("v"))
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _u[_qp];
  }
  else
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * getMaterialProperty<Real>("n_gas")[_qp] * _u[_qp];
}

Real
ReactantSecondOrder::computeQpJacobian()
{
  if (isCoupled("v"))
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _v[_qp] *
           _u[_qp] * _phi[_j][_qp];
  else
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * getMaterialProperty<Real>("n_gas")[_qp] *
           _u[_qp] * _phi[_j][_qp];
}

Real
ReactantSecondOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isCoupled("v"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _u[_qp] * _v[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
