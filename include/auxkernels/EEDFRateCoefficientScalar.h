#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"
#include "BoltzmannSolverScalar.h"

class EEDFRateCoefficientScalar : public AuxScalarKernel
{
public:
  EEDFRateCoefficientScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const BoltzmannSolverScalar & _data;
  int _reaction_number;
  bool _sample_value;
  const VariableValue & _sampler_var;
};
