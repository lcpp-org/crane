#include "ReactantThirdOrderLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactantThirdOrderLog);

template <>
InputParameters
validParams<ReactantThirdOrderLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "If coupled v == u.");
  params.addParam<bool>("_w_eq_u", false, "If coupled w == u.");
  return params;
}

ReactantThirdOrderLog::ReactantThirdOrderLog(const InputParameters & parameters)
  : Kernel(parameters),
    _reaction_coeff(getMaterialProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _v_id(coupled("v") ? coupled("v") : 0),
    _w_id(coupled("w") ? coupled("w") : 0),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u")),
    _v_coupled(isCoupled("v") ? true : false),
    _w_coupled(isCoupled("w") ? true : false),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactantThirdOrderLog::computeQpResidual()
{
  Real mult1,mult2;

  if (_v_coupled)
    mult1 = std::exp(_v[_qp]);
  else
    mult1 = _n_gas[_qp];

  if (_w_coupled)
    mult2 = std::exp(_w[_qp]);
  else
    mult2 = _n_gas[_qp];

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_u[_qp]) * mult1 * mult2;

  // if (isCoupled("v") && isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) * std::exp(_w[_qp]) * std::exp(_u[_qp]);
  // }
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) * _n_gas[_qp] * std::exp(_u[_qp]);
  // }
  // else if (!isCoupled("v") && isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_w[_qp]) * _n_gas[_qp] * std::exp(_u[_qp]);
  // }
  // else
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _n_gas[_qp] * std::exp(_u[_qp]);
}

Real
ReactantThirdOrderLog::computeQpJacobian()
{

  Real mult1, mult2;
  Real power, u_mult;
  // power = 0.0;
  // eq_u_mult = 1.0;
  // gas_mult = 1.0;

  if (_v_coupled)
    mult1 = std::exp(_v[_qp]);
  else
    mult1 = _n_gas[_qp];

  if (_w_coupled)
    mult2 = std::exp(_w[_qp]);
  else
    mult2 = _n_gas[_qp];

  u_mult = mult1 * mult2 * std::exp(_u[_qp]);

  power = 1;
  if (_v_coupled && _v_eq_u)
    power += 1;

  if (_w_coupled && _w_eq_u)
    power += 1;


  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power * u_mult * _phi[_j][_qp];

  // if (isCoupled("v") && isCoupled("w"))
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_v[_qp]) *
  //          std::exp(_w[_qp]) * _phi[_j][_qp];
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_v[_qp]) *
  //          _n_gas[_qp] * _phi[_j][_qp];
  // }
  // else if (!isCoupled("v") && isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_w[_qp]) *
  //          _n_gas[_qp] * _phi[_j][_qp];
  // }
  // else
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * _n_gas[_qp] *
  //          _n_gas[_qp] * _phi[_j][_qp];
}

Real
ReactantThirdOrderLog::computeQpOffDiagJacobian()
{
  Real mult1,mult2,u_mult,power;
  if (_v_coupled)
    mult1 = std::exp(_v[_qp]);
  else
    mult1 = _n_gas[_qp];

  if (_w_coupled)
    mult2 = std::exp(_w[_qp]);
  else
    mult2 = _n_gas[_qp];

  u_mult = mult1 * mult2 * std::exp(_u[_qp]);

  power = 0;

  if (_v_coupled && !_v_eq_u)
    power += 1;

  if (_w_coupled && !_w_eq_u)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power * u_mult * _phi[_j][_qp];
  // if (isCoupled("v") && isCoupled("w"))
  // {
  //   if (_v_id != _w_id)
  //   {
  //     if (jvar == _v_id)
  //       return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_u[_qp]) * std::exp(_w[_qp]) * _phi[_j][_qp];
  //     else if (jvar == _w_id)
  //       return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_u[_qp]) * std::exp(_v[_qp]) * _phi[_j][_qp];
  //     else
  //       return 0.0;
  //   }
  //   else
  //   {
  //     if (jvar == _v_id)
  //       return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_u[_qp]) * std::exp(_w[_qp]) * _phi[_j][_qp];
  //     else
  //       return 0.0;
  //   }
  // }
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   if (jvar == _v_id)
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_u[_qp]) * _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else if (!isCoupled("v") && isCoupled("w"))
  // {
  //   if (jvar == _v_id)
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 1.0 * std::exp(_u[_qp]) * _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else
  // {
  //   return 0.0;
  // }
}
