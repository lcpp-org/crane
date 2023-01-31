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

#include "GeneralUserObject.h"
#include "SplineInterpolation.h"

// class Function;

class ValueProvider : public GeneralUserObject
{
public:
  ValueProvider(const InputParameters & parameters);

  static InputParameters validParams();

  Real electron_temperature(const Real E_N) const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  SplineInterpolation _coefficient_interpolation;
  Real _rate_constant;

  std::string _sampling_format;
  std::string _rate_format;
};
