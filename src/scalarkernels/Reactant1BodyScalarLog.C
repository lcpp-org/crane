#include "Reactant1BodyScalarLog.h"

registerMooseObject("CraneApp", Reactant1BodyScalarLog);

template <>
InputParameters
validParams<Reactant1BodyScalarLog>()
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

Reactant1BodyScalarLog::Reactant1BodyScalarLog(const InputParameters & parameters)
  : ODEKernel(parameters),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _n_gas(getParam<Real>("n_gas")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
// _data(getUserObject<RateCoefficientProvider>("rate_provider"))
{
}

Real
Reactant1BodyScalarLog::computeQpResidual()
{
  return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(_u[_i]);
}

Real
Reactant1BodyScalarLog::computeQpJacobian()
{
  return -_stoichiometric_coeff * _rate_coefficient[_i] * std::exp(_u[_i]);
}

Real
Reactant1BodyScalarLog::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  return 0.0;
}
