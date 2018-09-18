#include "ProductSecondOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductSecondOrder);

template <>
InputParameters
validParams<ProductSecondOrder>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "Whether or not v and u are the same variable.");
  params.addParam<bool>("_w_eq_u", false, "Whether or not w and u are the same variable.");
  return params;
}

ProductSecondOrder::ProductSecondOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u"))
{
}

Real
ProductSecondOrder::computeQpResidual()
{
  Real mult_1, mult_2;

  if (isCoupled("v"))
    mult_1 = _u[_qp];
  else
    mult_1 = _n_gas[_qp];

  if (isCoupled("w"))
    mult_2 = _w[_qp];
  else
    mult_2 = _n_gas[_qp];

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult_1 * mult_2;
  // if (isCoupled("v") && isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _w[_qp];
  // }
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _n_gas[_qp];
  // }
  // else if (!isCoupled("v") && !isCoupled("w"))
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _w[_qp] * _n_gas[_qp];
  // }
  // else
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _n_gas[_qp];
  // }
}

Real
ProductSecondOrder::computeQpJacobian()
{
  Real mult_1, mult_2, power, eq_u_mult, gas_mult;
  power = 0.0;
  eq_u_mult = 1.0;
  gas_mult = 1.0;

  if (isCoupled("v"))
    mult_1 = _u[_qp];
  else
    mult_1 = _n_gas[_qp];

  if (isCoupled("w"))
    mult_2 = _w[_qp];
  else
    mult_2 = _n_gas[_qp];

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

  if (!_v_eq_u && !_w_eq_u)
  {
    return 0.0;
  }
  else
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * power * _reaction_coeff[_qp] * std::pow(eq_u_mult, power-1) * gas_mult * _phi[_j][_qp];
  }

  //
  // Real mult_1, mult_2;
  //
  // if (isCoupled("v"))
  //   mult_1 = _v[_qp];
  // else
  //   mult_1 = _n_gas[_qp];
  //
  // if (isCoupled("w"))
  //   mult_2 = _w[_qp];
  // else
  //   mult_2 = _n_gas[_qp];
  // //
  // // // Is this right?
  // if (!isCoupled("v") && !isCoupled("w"))
  //   return 0.0;
  // else if (_v_eq_u && !_w_eq_u)
  // {
  //   std::cout << -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult_2 * _phi[_j][_qp] << std::endl;
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult_2 * _phi[_j][_qp];
  //   // if (isCoupled("v") && !isCoupled("w"))
  //   // {
  //   //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _phi[_j][_qp];
  //   // }
  //   // else if (isCoupled("v") && isCoupled("w"))
  //   // {
  //   //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _w[_qp] * _phi[_j][_qp];
  //   // }
  //   // else
  //   //   return 0.0;
  // }
  // else if (!_v_eq_u && _w_eq_u)
  // {
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult_1 * _phi[_j][_qp];
  // }
  // else if (_v_eq_u && _w_eq_u)
  //   return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 2.0 * _u[_qp] * _phi[_j][_qp];
  // else
  //   return 0.0;
}

Real
ProductSecondOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Real mult_1, mult_2, power_1, power_2, eq_u_mult, gas_mult;
  // power_1 = 0.0;
  // power_2 = 0.0;
  // eq_u_mult = 1.0;
  // gas_mult = 1.0;
  //
  // if (isCoupled("v"))
  //   mult_1 = _v[_qp];
  // else
  //   mult_1 = _n_gas[_qp];
  //
  // if (isCoupled("w"))
  //   mult_2 = _w[_qp];
  // else
  //   mult_2 = _n_gas[_qp];
  //
  // if (_v_eq_u)
  // {
  //   power += 1.0;
  //   eq_u_mult *= mult_1;
  // }
  // else
  //   gas_mult *= mult_1;
  //
  // if (_w_eq_u)
  // {
  //   power += 1.0;
  //   eq_u_mult *= mult_2;
  // }
  // else
  //   gas_mult *= mult_2;

  return 0.0;
  //
  // if (isCoupled("v") && isCoupled("w"))
  // {
  //   if (jvar == _v_id && jvar == _w_id)
  //   {
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 2.0 * _v[_qp] * _phi[_j][_qp];
  //   }
  //   else if (jvar == _v_id && jvar != _w_id) // else if ((jvar == _v_id && jvar != _w_id) || (jvar == _v_id && !isCoupled("w"))) <- try this
  //   {
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _w[_qp] * _phi[_j][_qp];
  //   }
  //   else if (jvar != _v_id && jvar == _w_id)
  //   {
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _phi[_j][_qp];;
  //   }
  //   else
  //     return 0.0;
  // }
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   if (jvar == _v_id)
  //   {
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _phi[_j][_qp];
  //   }
  //   else
  //     return 0.0;
  // }
  // else if (!isCoupled("v") && isCoupled("w"))
  // {
  //   if (jvar == _w_id)
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else
  //   return 0.0;


  // This off-diagonal jacobian is incorrect, I think. How to fix?
  // if (isCoupled("w") && isCoupled("v"))
  // {
  //   if (_v_id != _w_id)
  //   {
  //     if (jvar == _v_id)
  //     {
  //       return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
  //               _w[_qp] * _phi[_j][_qp];
  //     }
  //     else if (jvar == _w_id)
  //     {
  //       return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
  //               _v[_qp] * _phi[_j][_qp];
  //     }
  //     else
  //       return 0.0;
  //   }
  //   else
  //   {
  //     if (jvar == _v_id)
  //     {
  //       return -_test[_i][_qp] * 2.0 * _stoichiometric_coeff * _reaction_coeff[_qp] *
  //               _v[_qp] * _w[_qp] * _phi[_j][_qp];
  //       // return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * 2.0 * _phi[_j][_qp] * (_v[_qp] + _w[_qp]);
  //     }
  //     else
  //     {
  //       return 0.0;
  //     }
  //   }
  // }
  // else if (isCoupled("v") && !isCoupled("w"))
  // {
  //   if (jvar == _v_id)
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
  //             _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else if (!isCoupled("v") && !isCoupled("w"))
  // {
  //   if (jvar == _w_id)
  //     return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
  //           _n_gas[_qp] * _phi[_j][_qp];
  //   else
  //     return 0.0;
  // }
  // else
  // {
  //   return 0.0;
  // }
}
