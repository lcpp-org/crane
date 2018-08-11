#include "ReactantSecondOrderMoles.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactantSecondOrderMoles);

template <>
InputParameters
validParams<ReactantSecondOrderMoles>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ReactantSecondOrderMoles::ReactantSecondOrderMoles(const InputParameters & parameters)
  : Kernel(parameters),
    _reaction_coeff(getMaterialProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactantSecondOrderMoles::computeQpResidual()
{
  if (isCoupled("v"))
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) * std::exp(_u[_qp]);
  }
  else
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * std::exp(_u[_qp]);
}

Real
ReactantSecondOrderMoles::computeQpJacobian()
{
  if (isCoupled("v"))
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_v[_qp]) *
           std::exp(_u[_qp]) * _phi[_j][_qp];
  else
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _n_gas[_qp] *
           std::exp(_u[_qp]) * _phi[_j][_qp];
}

Real
ReactantSecondOrderMoles::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (isCoupled("v"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_u[_qp]) * std::exp(_v[_qp]) * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
