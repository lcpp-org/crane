#include "ProductThirdOrderLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductThirdOrderLog);

template <>
InputParameters
validParams<ProductThirdOrderLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting.");
  params.addCoupledVar("w", "The second variable that is reacting.");
  params.addCoupledVar("x", "The third variable that is reacting.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "If coupled v == u.");
  params.addParam<bool>("_w_eq_u", false, "If coupled w == u.");
  params.addParam<bool>("_x_eq_u", false, "If coupled x == u.");
  return params;
}

ProductThirdOrderLog::ProductThirdOrderLog(const InputParameters & parameters)
  : Kernel(parameters),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _x(isCoupled("x") ? coupledValue("x") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    _x_id(isCoupled("x") ? coupled("x") : 0),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u")),
    _x_eq_u(getParam<bool>("_x_eq_u")),
    _v_coupled(isCoupled("v") ? true : false),
    _w_coupled(isCoupled("w") ? true : false),
    _x_coupled(isCoupled("x") ? true : false),
    // _n_gas(getMaterialProperty<Real>("n_gas")),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ProductThirdOrderLog::computeQpResidual()
{
  // Real mult1, mult2, mult3;
  // if (_v_coupled)
  //  mult1 = std::exp(_v[_qp]);
  // else
  //  mult1 = _n_gas[_qp];

  // if (_w_coupled)
  //  mult2 = std::exp(_w[_qp]);
  // else
  //  mult2 = _n_gas[_qp];

  // if (_x_coupled)
  //  mult3 = std::exp(_x[_qp]);
  // else
  //  mult3 = _n_gas[_qp];

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) *
         std::exp(_w[_qp]) * std::exp(_x[_qp]);
  // return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult1 * mult2 * mult3;
  // if (isCoupled("w") && isCoupled("v") && isCoupled("x"))
  // {
  //   return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_v[_qp]) *
  //   std::exp(_w[_qp]) * std::exp(_x[_qp]);
  // }
  // else if (isCoupled("v") && !isCoupled("w") && isCoupled("x"))
  // {
  //   return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_v[_qp]) *
  //   _n_gas[_qp] * std::exp(_x[_qp]);
  // }
  // else if (!isCoupled("v") && !isCoupled("w") && isCoupled("x"))
  // {
  //   return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_w[_qp]) *
  //   _n_gas[_qp];
  // }
  // else
  // {
  //   return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * _n_gas[_qp] *
  //   _n_gas[_qp];
  // }
  // Need to add more possibilities
}

Real
ProductThirdOrderLog::computeQpJacobian()
{
  //  Real mult1, mult2, mult3, power, u_mult;
  Real power;
  // if (_v_coupled)
  //  mult1 = std::exp(_v[_qp]);
  // else
  //  mult1 = _n_gas[_qp];

  // if (_w_coupled)
  //  mult2 = std::exp(_w[_qp]);
  // else
  //  mult2 = _n_gas[_qp];

  // if (_x_coupled)
  //  mult3 = std::exp(_x[_qp]);
  // else
  //  mult3 = _n_gas[_qp];

  // u_mult = mult1 * mult2 * mult3; // This is true regardless of derivatives!

  power = 0;
  if (_v_coupled && _v_eq_u)
    power += 1;
  if (_w_coupled && _w_eq_u)
    power += 1;
  if (_x_coupled && _x_eq_u)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power *
         std::exp(_v[_qp]) * std::exp(_w[_qp]) * std::exp(_x[_qp]) * _phi[_j][_qp];
}

Real
ProductThirdOrderLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Real mult1, mult2, mult3, power, u_mult;
  Real power;
  // if (_v_coupled)
  //  mult1 = std::exp(_v[_qp]);
  // else
  //  mult1 = _n_gas[_qp];

  // if (_w_coupled)
  //  mult2 = std::exp(_w[_qp]);
  // else
  //  mult2 = _n_gas[_qp];

  // if (_x_coupled)
  //  mult3 = std::exp(_x[_qp]);
  // else
  //  mult3 = _n_gas[_qp];
  // u_mult = mult1 * mult2 * mult3;
  power = 0;

  if (_v_coupled && !_v_eq_u && jvar == _v_id)
    power += 1;

  if (_w_coupled && !_w_eq_u && jvar == _w_id)
    power += 1;

  if (_x_coupled && !_x_eq_u && jvar == _x_id)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) *
         std::exp(_w[_qp]) * std::exp(_x[_qp]) * power * _phi[_j][_qp];
  // This jacobian is incorrect, I think. How to fix? - S. Keniley, 4/19/2018
  // if (isCoupled("v") && isCoupled("w") && isCoupled("x"))
  // {
  //   if (_v_id != _w_id && _v_id != _x_id && _w_id != _x_id)
  //   {
  //     if (jvar == _v_id)
  //     {
  //       return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
  //               std::exp(_w[_qp]) * _phi[_j][_qp];
  //     }
  //     else if (jvar == _w_id)
  //     {
  //       return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
  //               std::exp(_v[_qp]) * _phi[_j][_qp];
  //     }
  //     else
  //       return 0.0;
  //   }
  //   else if (_v_id != _w_id && _v_id != _x_id && _w_id == _x_id)
  //   {
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
  //       std::exp(_v[_qp]) * 2.0 * std::exp(_w[_qp]) * std::exp(_x[_qp]) * _phi[_j][_qp];
  //   }
  //   else
  //   {
  //     if (jvar == _v_id)
  //     {
  //       return -_test[_i][_qp] * 2.0 * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
  //               std::exp(_v[_qp]) * std::exp(_w[_qp]) * _phi[_j][_qp];
  //     }
  //     else
  //     {
  //       return 0.0;
  //     }
  //   }
  // }
  // else if (!isCoupled("w") && isCoupled("v") && isCoupled("x"))
  // {
  //   if (jvar == _v_id)
  //     return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
  //             _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else if (isCoupled("w") && !isCoupled("v") && isCoupled("x"))
  // {
  //   if (jvar == _w_id)
  //     return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
  //           _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else
  // {
  //   return 0.0;
  // }
}
