#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"
#include "BoltzmannSolverScalar.h"

class BolsigValueScalar : public AuxScalarKernel
{
public:
  BolsigValueScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const BoltzmannSolverScalar & _data;
  std::string _data_type;
  bool _sample_value;
  const VariableValue & _sampler_var;
};
