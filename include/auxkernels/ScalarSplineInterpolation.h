//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
