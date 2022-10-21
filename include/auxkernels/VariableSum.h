#pragma once

#include "AuxScalarKernel.h"
// #include "SplineInterpolation.h"

class VariableSum : public AuxScalarKernel
{
public:
  VariableSum(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  unsigned int _nargs;
  std::vector<const VariableValue *> _args;
};
