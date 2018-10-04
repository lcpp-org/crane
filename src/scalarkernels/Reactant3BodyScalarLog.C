#include "Reactant3BodyScalarLog.h"

registerMooseObject("CraneApp", Reactant3BodyScalarLog);

template <>
InputParameters
validParams<Reactant3BodyScalarLog>()
{
  InputParameters params = validParams<ODEKernel>();
  params.addCoupledVar("v", 0, "Coupled variable 1.");
  params.addCoupledVar("w", 0, "Coupled variable 2.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("n_gas", "The gas density.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  // params.addRequiredParam<Real>("reaction_coefficient", "The reaction coefficient.");
  params.addParam<bool>("v_eq_u", false, "Whether or not u = v.");
  params.addParam<bool>("w_eq_u", false, "Whether or not u = v.");
  params.addParam<bool>("rate_constant_equation", false, "True if rate constant is provided by equation.");
  // params.addRequiredParam<UserObjectName>("rate_provider",
      // "The name of the UserObject that can provide the rate coefficient.");
  return params;
}

Reactant3BodyScalarLog::Reactant3BodyScalarLog(const InputParameters & parameters)
  : ODEKernel(parameters),
    _v_var(isCoupledScalar("v") ? coupledScalar("v") : 0),
    _v(coupledScalarValue("v")),
    _w_var(isCoupledScalar("w") ? coupledScalar("w") : 0),
    _w(coupledScalarValue("w")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    // _reaction_coeff(getParam<Real>("reaction_coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _w_eq_u(getParam<bool>("w_eq_u")),
    _v_coupled(isCoupledScalar("v") ? true : false),
    _w_coupled(isCoupledScalar("w") ? true : false),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
    // _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Reactant3BodyScalarLog::computeQpResidual()
{
  Real mult1, mult2;
  if (_v_coupled)
    mult1 = std::exp(_v[_i]);
  else
    mult1 = _n_gas;

  if (_w_coupled)
    mult2 = std::exp(_w[_i]);
  else
    mult2 = _n_gas;

  // if (_rate_constant_equation)
  return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(_u[_i]) * mult1 * mult2;
  // else
    // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i] * mult1 * mult2;

  // return -_stoichiometric_coeff * _reaction_coeff * mult1 * mult2;
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i] * mult1 * mult2;
  // return -_stoichiometric_coeff * 1e-16 * _n_gas * _n_gas;
}

Real
Reactant3BodyScalarLog::computeQpJacobian()
{
  Real mult1, mult2;
  Real power, u_mult, gas_mult;
  Real rate_constant;
  // power = 0.0;
  // eq_u_mult = 1.0;
  // gas_mult = 1.0;

  if (_v_coupled)
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (_w_coupled)
    mult2 = _w[_i];
  else
    mult2 = _n_gas;

  u_mult = _u[_i];
  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
    // rate_constant = _data.reaction_coefficient();

  // if (_v_eq_u && _w_eq_u)
  // {
  //   return -_stoichiometric_coeff * rate_constant * 3.0 * std::pow(_u[_i], 2.0);
  // }
  // else if (_v_eq_u && !_w_eq_u)
  // {
  //   return -_stoichiometric_coeff * rate_constant * 2.0 * _u[_i] * mult2;
  // }
  // else if (!_v_eq_u && _w_eq_u)
  // {
  //   return -_stoichiometric_coeff * rate_constant * 2.0 * _u[_i] * mult1;
  // }
  // else
  // {
  //   return -_stoichiometric_coeff * rate_constant * mult1 * mult2;
  // }

  power = 1;
  gas_mult = 0;
  if (_v_coupled && _v_eq_u)
  {
    power += 1;
    u_mult += mult1;
  }
  else
    gas_mult += mult1;
  if (_w_coupled && _w_eq_u)
  {
    power += 1;
    u_mult += mult2;
  }
  else
    gas_mult += mult2;

  return -_stoichiometric_coeff * rate_constant * std::exp(gas_mult) * power * std::exp(u_mult);


}

Real
Reactant3BodyScalarLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real mult1,mult2;
  Real rate_constant,power,off_diag,gas_mult;

  if (_v_coupled)
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  if (_w_coupled)
    mult2 = _w[_i];
  else
    mult2 = _n_gas;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
    // rate_constant = _data.reaction_coefficient();


  power = 0.0;
  off_diag = 0.0;
  gas_mult = 0.0;

  if (_v_coupled && !_v_eq_u && jvar==_v_var)
  {
    power += 1;
    off_diag += mult1;
  }
  else
    gas_mult += mult1;

  if (_w_coupled && !_w_eq_u && jvar==_w_var)
  {
    power += 1;
    off_diag += mult2;
  }
  else
    gas_mult += mult2;

  return -_stoichiometric_coeff * rate_constant * std::exp(_u[_i]) * std::exp(gas_mult) * power * std::exp(off_diag);
  // if (isCoupledScalar("v") && isCoupledScalar("w"))
  // {
  //   if (!_v_eq_u && !_w_eq_u)
  //   {
  //     if (_v_var != _w_var)
  //     {
  //       if (jvar == _v_var)
  //         return -_stoichiometric_coeff * rate_constant * _u[_i] * mult2;
  //       else if (jvar == _w_var)
  //         return -_stoichiometric_coeff * rate_constant * _u[_i] * mult1;
  //       else
  //         return 0.0;
  //     }
  //     else
  //     {
  //       if (jvar == _v_var)
  //         return -_stoichiometric_coeff * rate_constant * _u[_i] * 2.0 * mult1;
  //       else
  //         return 0.0;
  //     }
  //   }
  //   else
  //     return 0.0;
  // }
  // else if (isCoupledScalar("v") && !isCoupledScalar("w"))
  // {
  //   if (jvar == _v_var && !_v_eq_u)
  //   {
  //     return -_stoichiometric_coeff * rate_constant * _u[_i] * mult2;
  //   }
  //   else
  //     return 0.0;
  // }
  // else if (!isCoupledScalar("v") && isCoupledScalar("w"))
  // {
  //   if (jvar == _w_var && !_w_eq_u)
  //     return -_stoichiometric_coeff * rate_constant * _u[_i] * mult1;
  //   else
  //     return 0.0;
  // }
  // else
  //   return 0.0;



}
