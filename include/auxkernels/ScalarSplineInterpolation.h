#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"

class ScalarSplineInterpolation : public AuxScalarKernel
{
public:
  ScalarSplineInterpolation(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  SplineInterpolation _coefficient_interpolation;
  const VariableValue & _sampler_var;
  Real _sampler_const;
  std::string _sampling_format;
  bool _use_time;
  bool _use_log;
  std::string _interpolation_type;
  Real _scale_factor;
};
