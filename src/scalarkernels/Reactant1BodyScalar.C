#include "Reactant1BodyScalar.h"

registerMooseObject("CraneApp", Reactant1BodyScalar);

template <>
InputParameters
validParams<Reactant1BodyScalar>()
{
  InputParameters params = validParams<ODEKernel>();
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("n_gas", "The gas density.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  params.addParam<bool>(
      "rate_constant_equation", false, "True if rate constant is provided by equation.");
  // params.addRequiredParam<UserObjectName>("rate_provider",
  // "The name of the UserObject that can provide the rate coefficient.");
  return params;
}

Reactant1BodyScalar::Reactant1BodyScalar(const InputParameters & parameters)
  : ODEKernel(parameters),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
// _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Reactant1BodyScalar::computeQpResidual()
{
  // if (_rate_constant_equation)
  return -_stoichiometric_coeff * _rate_coefficient[_i] * _u[_i];
  // else
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i];
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i];
}

Real
Reactant1BodyScalar::computeQpJacobian()
{
  Real rate_constant;
  // if (_rate_constant_equation)
  rate_constant = _rate_coefficient[_i];
  // else
  // rate_constant = _data.reaction_coefficient();

  return -_stoichiometric_coeff * rate_constant;
}

Real
Reactant1BodyScalar::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.0;
}
