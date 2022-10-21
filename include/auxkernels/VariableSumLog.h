#pragma once

#include "AuxScalarKernel.h"
// #include "SplineInterpolation.h"

class VariableSumLog : public AuxScalarKernel
{
public:
  VariableSumLog(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  unsigned int _nargs;
  std::vector<const VariableValue *> _args;
};
