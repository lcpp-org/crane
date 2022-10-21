#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"

class AuxInitialConditionScalar : public AuxScalarKernel
{
public:
  AuxInitialConditionScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  Real _value;
};
