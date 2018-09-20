#include "ProductThirdOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductThirdOrder);

template <>
InputParameters
validParams<ProductThirdOrder>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting.");
  params.addCoupledVar("w", "The second variable that is reacting.");
  params.addCoupledVar("x", "The third variable that is reacting.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "Whether or not v and u are the same variable.");
  params.addParam<bool>("_w_eq_u", false, "Whether or not w and u are the same variable.");
  params.addParam<bool>("_x_eq_u", false, "Whether or not x and u are the same variable.");
  return params;
}

ProductThirdOrder::ProductThirdOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _x(isCoupled("x") ? coupledValue("x") : _zero),
    _v_id(coupled("v") ? coupled("w") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    _x_id(isCoupled("x") ? coupled("x") : 0),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u")),
    _x_eq_u(getParam<bool>("_x_eq_u"))
{
}

Real
ProductThirdOrder::computeQpResidual()
{
  // Here we see if the variables are coupled or not
  // If not, the values default to _n_gas.
  // (This will need to be changed eventually.)
  Real mult_1, mult_2, mult_3;

  if (isCoupled("v"))
    mult_1 = _v[_qp];
  else
    mult_1 = _n_gas[_qp];

  if (isCoupled("w"))
    mult_2 = _w[_qp];
  else
    mult_2 = _n_gas[_qp];

  if (isCoupled("x"))
    mult_3 = _x[_qp];
  else
    mult_3 = _n_gas[_qp];

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult_1 * mult_2 * mult_3;
  // if (isCoupled("v") && isCoupled("w") && isCoupled("x"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _w[_qp] * _x[_qp];
  // }
  // else if (isCoupled("v") && !isCoupled("w") && isCoupled("x"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _n_gas[_qp] * _x[_qp];
  // }
  // else if (!isCoupled("v") && isCoupled("w") && isCoupled("x"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _w[_qp] * _n_gas[_qp] * _x[_qp];
  // }
  // else if (isCoupled("v") && !isCoupled("w") && !isCoupled("x"))
  // {
  //
  // }
  // else
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _n_gas[_qp] * _x[_qp];
  // }
}

Real
ProductThirdOrder::computeQpJacobian()
{
  Real mult_1, mult_2, mult_3, power, eq_u_mult, gas_mult;
  power = 0.0;
  eq_u_mult = 1.0;
  gas_mult = 1.0;

  if (isCoupled("v"))
    mult_1 = _v[_qp];
  else
    mult_1 = _n_gas[_qp];

  if (isCoupled("w"))
    mult_2 = _w[_qp];
  else
    mult_2 = _n_gas[_qp];

  if (isCoupled("x"))
    mult_3 = _x[_qp];
  else
    mult_3 = _n_gas[_qp];

  if (_v_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_1;
  }
  else
    gas_mult *= mult_1;

  if (_w_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_2;
  }
  else
    gas_mult *= mult_2;

  if (_x_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_3;
  }
  else
    gas_mult *= mult_3;

  if (!_v_eq_u && !_w_eq_u && !_x_eq_u)
    return 0.0;
  else
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * power * std::pow(eq_u_mult, power-1) * gas_mult * _phi[_j][_qp];
  }
  // return 0.0;
  // if (_v_eq_u && _w_eq_u && _x_eq_u)
    // return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
}

Real
ProductThirdOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  // This jacobian is incorrect, I think. How to fix? - S. Keniley, 4/19/2018
  if (isCoupled("w") && isCoupled("v"))
  {
    if (_v_id != _w_id)
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
                _w[_qp] * _phi[_j][_qp];
      }
      else if (jvar == _w_id)
      {
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
                _v[_qp] * _phi[_j][_qp];
      }
      else
        return 0.0;
    }
    else
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * 2.0 * _stoichiometric_coeff * _reaction_coeff[_qp] *
                _v[_qp] * _w[_qp] * _phi[_j][_qp];
      }
      else
      {
        return 0.0;
      }
    }
  }
  else if (!isCoupled("w") && isCoupled("v"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
              _n_gas[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else if (isCoupled("w") && !isCoupled("v"))
  {
    if (jvar == _w_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
            _n_gas[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
