#pragma once

#include "AuxKernel.h"
#include "SplineInterpolation.h"
#include "LinearInterpolation.h"

class DataRead : public AuxKernel
{
public:
  DataRead(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  SplineInterpolation _coefficient_interpolation;
  // LinearInterpolation _coefficient_interpolation_linear;
  const VariableValue & _sampler_var;
  Real _sampler_const;
  std::string _sampling_format;
  bool _use_time;
  bool _use_log;
  Real _scale_factor;
};
