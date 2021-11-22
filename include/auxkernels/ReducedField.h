#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"

class ReducedField : public AuxScalarKernel
{
public:
  ReducedField(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~ReducedField();

protected:
  virtual Real computeValue();

  const VariableValue & _mobility;
};
