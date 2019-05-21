#include "Reactant2BodyScalar.h"

registerMooseObject("CraneApp", Reactant2BodyScalar);

template <>
InputParameters
validParams<Reactant2BodyScalar>()
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

Reactant2BodyScalar::Reactant2BodyScalar(const InputParameters & parameters)
  : ODEKernel(parameters),
    _v_var(isCoupledScalar("v") ? coupledScalar("v") : -1),
    _v(coupledScalarValue("v")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
    // _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Reactant2BodyScalar::computeQpResidual()
{
  Real mult1;
  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  // if (_rate_constant_equation)
  return -_stoichiometric_coeff * _rate_coefficient[_i] * _u[_i] * mult1;
  // else
    // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i] * mult1;
}

Real
Reactant2BodyScalar::computeQpJacobian()
{
  Real mult1;
  Real rate_constant;
  if (isCoupledScalar("v"))
    mult1 = _v[_i];
  else
    mult1 = _n_gas;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
    // rate_constant = _data.reaction_coefficient();

  if (_v_eq_u)
  {
    return -_stoichiometric_coeff * rate_constant * 2.0 * _u[_i];
  }
  else
    return -_stoichiometric_coeff * rate_constant * mult1;
  // if (_v_eq_u)
  // {
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * 2.0 * _u[_i];
  // }
  // else
  //   return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
}

Real
Reactant2BodyScalar::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real rate_constant;

  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
    // rate_constant = _data.reaction_coefficient();

  if (isCoupledScalar("v"))
  {
    if (jvar == _v_var && !_v_eq_u)
    {
      return -_stoichiometric_coeff * rate_constant * _u[_i];
    }
    else
      return 0.0;
  }
  else
    return 0.0;

}
