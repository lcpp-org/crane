#pragma once

#include "AuxScalarKernel.h"
#include "LinearInterpolation.h"

class ScalarLinearInterpolation : public AuxScalarKernel
{
public:
  ScalarLinearInterpolation(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  LinearInterpolation _coefficient_interpolation;
  const VariableValue & _sampler_var;
  Real _sampler_const;
  std::string _sampling_format;
  bool _use_time;
  bool _use_log;
  Real _scale_factor;
};
