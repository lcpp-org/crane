#include "ReactantThirdOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactantThirdOrder);

template <>
InputParameters
validParams<ReactantThirdOrder>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("v_eq_u", false, "If v == u.");
  params.addParam<bool>("w_eq_u", false, "If w == u.");
  return params;
}

ReactantThirdOrder::ReactantThirdOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _reaction_coeff(getMaterialProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _w_eq_u(getParam<bool>("w_eq_u"))
{
}

Real
ReactantThirdOrder::computeQpResidual()
{
  if (isCoupled("v") && isCoupled("w"))
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _w[_qp] * _u[_qp];
  }
  else if (isCoupled("v") && !isCoupled("w"))
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _n_gas[_qp] * _u[_qp];
  }
  else if (!isCoupled("v") && isCoupled("w"))
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _w[_qp] * _n_gas[_qp] * _u[_qp];
  }
  else
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _n_gas[_qp] * _u[_qp];
}

Real
ReactantThirdOrder::computeQpJacobian()
{
  Real mult1, mult2, power, gas_mult;
  if (isCoupled("v"))
    mult1 = _v[_qp];
  else
    mult1 = _n_gas[_qp];

  if (isCoupled("w"))
    mult2 = _w[_qp];
  else
    mult2 = _n_gas[_qp];

  power = 1.0;
  // eq_u_mult = _u[_qp];  // multiplies variables that are equal to _u
  gas_mult = 1.0; // multiplies the rest of the variables (!= _u)
  if (_v_eq_u)
  {
    power += 1.0;
    // eq_u_mult *= mult1;
  }
  else
    gas_mult *= mult1;

  if (_w_eq_u)
  {
    power += 1.0;
    // eq_u_mult *= mult2;
  }
  else
    gas_mult *= mult2;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power * std::pow(_u[_qp], power-1) * gas_mult * _phi[_j][_qp];

  // if (isCoupled("v") && isCoupled("w"))
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _v[_qp] *
  //          _w[_qp] * _phi[_j][_qp];
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _v[_qp] *
  //          _n_gas[_qp] * _phi[_j][_qp];
  // }
  // else if (!isCoupled("v") && isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _w[_qp] *
  //          _n_gas[_qp] * _phi[_j][_qp];
  // }
  // else
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _n_gas[_qp] *
  //          _n_gas[_qp] * _phi[_j][_qp];
}

Real
ReactantThirdOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real mult1, mult2;
  if (isCoupled("v"))
    mult1 = _v[_qp];
  else
    mult1 = _n_gas[_qp];

  if (isCoupled("w"))
    mult2 = _w[_qp];
  else
    mult2 = _n_gas[_qp];


  if (isCoupled("v") && isCoupled("w"))
  {
    if (_v_id != _w_id)
    {
      if ((jvar == _v_id) && (!_v_eq_u))
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _u[_qp] * mult2 * _phi[_j][_qp];
      else if ((jvar == _w_id) && (!_w_eq_u))
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _u[_qp] * mult1 * _phi[_j][_qp];
      else
        return 0.0;
    }
    else
    {
      if (jvar == _v_id)
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 2.0 * _u[_qp] * mult1 * _phi[_j][_qp];
      else
        return 0.0;
    }
  }
  else if (isCoupled("v") && !isCoupled("w"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _u[_qp] * mult2 * _phi[_j][_qp];
    else
      return 0.0;
  }
  else if (!isCoupled("v") && isCoupled("w"))
  {
    if (jvar == _w_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _u[_qp] * mult1 * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
