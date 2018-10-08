#include "Reactant2BodyScalarLog.h"

registerMooseObject("CraneApp", Reactant2BodyScalarLog);

template <>
InputParameters
validParams<Reactant2BodyScalarLog>()
{
  InputParameters params = validParams<ODEKernel>();
  params.addCoupledVar("v", 0, "Coupled variable 1.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("n_gas", "The gas density.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  params.addParam<bool>("v_eq_u", false, "Whether or not u = v.");
  params.addParam<bool>("rate_constant_equation", false, "True if rate constant is provided by equation.");
  // params.addRequiredParam<UserObjectName>("rate_provider",
      // "The name of the UserObject that can provide the rate coefficient.");
  return params;
}

Reactant2BodyScalarLog::Reactant2BodyScalarLog(const InputParameters & parameters)
  : ODEKernel(parameters),
    _v_var(isCoupledScalar("v") ? coupledScalar("v") : 0),
    _v(coupledScalarValue("v")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _v_coupled(isCoupledScalar("v") ? true : false),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
    // _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Reactant2BodyScalarLog::computeQpResidual()
{
  Real mult1;
  if (_v_coupled)
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  // if (_rate_constant_equation)
  // return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(_u[_i]) * std::exp(mult1);

  return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(_u[_i] + mult1);
  // else
    // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i] * mult1;
}

Real
Reactant2BodyScalarLog::computeQpJacobian()
{
  Real mult1,power,u_mult,gas_mult;
  // Real rate_constant;
  if (_v_coupled)
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  u_mult = _u[_i];
  power = 1;
  gas_mult = 0;
  if (_v_coupled && _v_eq_u)
  {
    power += 1;
    u_mult += mult1;
  }
  else
    gas_mult += mult1;

  // return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(gas_mult) * power * std::exp(u_mult);
  return -_stoichiometric_coeff * _rate_coefficient[_i] * power * std::exp(u_mult + gas_mult);

  // if (_rate_constant_equation)
  // rate_constant = _rate_coefficient[_i];
  // else
    // rate_constant = _data.reaction_coefficient();

  // if (_v_eq_u)
  // {
  //   return -_stoichiometric_coeff * rate_constant * 2.0 * _u[_i];
  // }
  // else
  //   return -_stoichiometric_coeff * rate_constant * mult1;
  // if (_v_eq_u)
  // {
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * 2.0 * _u[_i];
  // }
  // else
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
}

Real
Reactant2BodyScalarLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real mult1,power,off_diag,gas_mult;
  // Real rate_constant;
  if (_v_coupled)
    mult1 = _v[_i];
  else
    mult1 = _n_gas;


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

  return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(_u[_i]) * std::exp(gas_mult) * power * std::exp(off_diag);

  // if (_rate_constant_equation)
  // rate_constant = _rate_coefficient[_i];
  // else
    // rate_constant = _data.reaction_coefficient();
  //
  // if (isCoupledScalar("v"))
  // {
  //   if (jvar == _v_var && !_v_eq_u)
  //   {
  //     return -_stoichiometric_coeff * rate_constant * _u[_i];
  //   }
  //   else
  //     return 0.0;
  // }
  // else
  //   return 0.0;

}
