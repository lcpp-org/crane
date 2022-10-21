#include "MoleFraction.h"

registerMooseObject("CraneApp", MoleFraction);

InputParameters
MoleFraction::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addCoupledVar("neutral_density", "The neutral gas density (calculated by VariableSum).");
  params.addCoupledVar("species_density", "The species density (nonlinear variable).");
  return params;
}

MoleFraction::MoleFraction(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _neutral_density(coupledScalarValue("neutral_density")),
    _species_density(coupledScalarValue("species_density"))
{
}

Real
MoleFraction::computeValue()
{
  return _species_density[_i] / _neutral_density[_i];
}
