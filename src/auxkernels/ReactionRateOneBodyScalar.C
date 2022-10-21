#include "ReactionRateOneBodyScalar.h"

registerMooseObject("CraneApp", ReactionRateOneBodyScalar);

InputParameters
ReactionRateOneBodyScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ReactionRateOneBodyScalar::ReactionRateOneBodyScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _v(coupledScalarValue("v")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactionRateOneBodyScalar::computeValue()
{
  return _stoichiometric_coeff * _rate_coefficient[_i] * _v[_i];
}
