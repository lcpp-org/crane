#include "Product2BodyScalar.h"

registerMooseObject("CraneApp", Product2BodyScalar);

template <>
InputParameters
validParams<Product2BodyScalar>()
{
  InputParameters params = validParams<ODEKernel>();
  params.addCoupledVar("v", 0, "Coupled variable 1.");
  params.addCoupledVar("w", 0, "Coupled variable 2.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("n_gas", "The gas density.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  // params.addRequiredParam<Real>("reaction_coefficient", "The reaction coefficient.");
  params.addParam<bool>("v_eq_u", false, "Whether or not u = v.");
  params.addParam<bool>("w_eq_u", false, "Whether or not u = w.");
  params.addParam<bool>("rate_constant_equation", false, "True if rate constant is provided by equation.");
  // params.addRequiredParam<UserObjectName>("rate_provider",
  //     "The name of the UserObject that can provide the rate coefficient.");
  return params;
}

Product2BodyScalar::Product2BodyScalar(const InputParameters & parameters)
  : ODEKernel(parameters),
    _v_var(isCoupledScalar("v") ? coupledScalar("v") : -1),
    _v(coupledScalarValue("v")),
    _w_var(isCoupledScalar("w") ? coupledScalar("w") : -1),
    _w(coupledScalarValue("w")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    // _reaction_coeff(getParam<Real>("reaction_coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _w_eq_u(getParam<bool>("w_eq_u")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
    // _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Product2BodyScalar::computeQpResidual()
{
  Real mult1, mult2;
  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (isCoupledScalar("w"))
    mult2 = _w[_i];
  else
    mult2 = _n_gas;

  // if (_rate_constant_equation)
  return -_stoichiometric_coeff * _rate_coefficient[_i] * mult1 * mult2;
  // else
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1 * mult2;
  // return -_stoichiometric_coeff * _reaction_coeff * mult1 * mult2;
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1 * mult2;
  // return -_stoichiometric_coeff * 1e-16 * _n_gas * _n_gas;
}

Real
Product2BodyScalar::computeQpJacobian()
{
  Real mult1, mult2;
  Real power, eq_u_mult, gas_mult;
  Real rate_constant;
  power = 0.0;
  eq_u_mult = 1.0;
  gas_mult = 1.0;

  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (isCoupledScalar("w"))
    mult2 = _w[_i];
  else
    mult2 = _n_gas;

  if (isCoupledScalar("v") && _v_eq_u)
  {
    power += 1.0;
    eq_u_mult = mult1;
  }
  else
    gas_mult *= mult1;

  if (isCoupledScalar("w") && _w_eq_u)
  {
    power += 1.0;
    eq_u_mult = mult2;
  }
  else
    gas_mult *= mult2;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
  //   rate_constant = _data.reaction_coefficient();
  // std::cout << -_stoichiometric_coeff * rate_constant * gas_mult * power * std::pow(eq_u_mult, power-1) << std::endl;
  return -_stoichiometric_coeff * rate_constant * gas_mult * power * std::pow(eq_u_mult, power-1);
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * gas_mult * power * eq_u_mult;
  // return 0.0;
  // if (_v_eq_u && _w_eq_u)
  // {
  //   // return -_stoichiometric_coeff * 1e-16 * 2.0 * _u[_i];
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * 2.0 * _u[_i];
  // }
  // else if (_v_eq_u && !_w_eq_u)
  // {
  //   // return -_stoichiometric_coeff * 1e-16 * mult2;
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * mult2;
  // }
  // else if (!_v_eq_u && _w_eq_u)
  // {
  //   // return -_stoichiometric_coeff * 1e-16 * mult1;
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
  // }
  // else
  // {
  //   return 0.0;
  // }

}

Real
Product2BodyScalar::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power, mult1, mult2, deriv_factor, other_factor;
  Real rate_constant;
  power = 0;
  other_factor = 1;
  deriv_factor = 1;
  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (isCoupledScalar("w"))
    mult2 = _w[_i];
  else
    mult2 = _n_gas;

  if (jvar == _v_var && !_v_eq_u)
  {
    power += 1;
    deriv_factor = mult1;
  }
  else
    other_factor *= mult1;

  if (jvar == _w_var && !_w_eq_u)
  {
    power += 1;
    deriv_factor = mult2;
  }
  else
    other_factor *= mult2;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // std::cout << -_stoichiometric_coeff * rate_constant * other_factor * power * std::pow(deriv_factor, power-1) << std::endl;
  // else
  //   rate_constant = _data.reaction_coefficient();
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * other_factor * power * deriv_factor;
  // std::cout << -_stoichiometric_coeff * _data.reaction_coefficient() * other_factor * power
    // * std::pow(deriv_factor, power-1) << std::endl;
  return -_stoichiometric_coeff * rate_constant * other_factor * power
    * std::pow(deriv_factor, power-1);

  // return -_stoichiometric_coeff * _data.reaction_coefficient() * (v_power + w_power) *
  //   std::pow(mult1, v_power - 1) * std::pow(mult2, w_power-1);

  // return 0;
  // Real mult1,mult2;
  //
  // if (isCoupledScalar("v"))
  //   mult1 = _v[_i];
  // else
  //   mult1 = _n_gas;
  //
  // if (isCoupledScalar("w"))
  //   mult2 = _w[_i];
  // else
  //   mult2 = _n_gas;
  //
  // if (isCoupledScalar("v") && isCoupledScalar("w"))
  // {
  //   if (!_v_eq_u && !_w_eq_u)
  //   {
  //     if (_v_var != _w_var)
  //     {
  //       if (jvar == _v_var)
  //         return -_stoichiometric_coeff * _data.reaction_coefficient() * mult2;
  //       else if (jvar == _w_var)
  //         return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
  //       else
  //         return 0.0;
  //     }
  //     else
  //     {
  //       if (jvar == _v_var)
  //         return -_stoichiometric_coeff * _data.reaction_coefficient() * 2.0 * mult1;
  //       else
  //         return 0.0;
  //     }
  //   }
  //   else
  //     return 0.0;
  // }
  // else if (isCoupledScalar("v") && !isCoupledScalar("w"))
  // {
  //   if (jvar == _v_var)
  //   {
  //     return -_stoichiometric_coeff * _data.reaction_coefficient() * mult2;
  //   }
  //   else
  //     return 0.0;
  // }
  // else if (!isCoupledScalar("v") && isCoupledScalar("w"))
  // {
  //   if (jvar == _w_var)
  //     return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
  //   else
  //     return 0.0;
  // }
  // else
  //   return 0.0;

}
