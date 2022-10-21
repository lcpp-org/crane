#include "AuxInitialConditionScalar.h"

registerMooseObject("CraneApp", AuxInitialConditionScalar);

InputParameters
AuxInitialConditionScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addParam<Real>("initial_condition", 0, "The initial value of the auxiliary variable.");
  return params;
}

AuxInitialConditionScalar::AuxInitialConditionScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _value(getParam<Real>("initial_condition"))
{
}

Real
AuxInitialConditionScalar::computeValue()
{
  return _value;
}
