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

class RateCoefficientProvider : public GeneralUserObject
{
public:
  RateCoefficientProvider(const InputParameters & parameters);

  static InputParameters validParams();

  Real reaction_coefficient() const;
  Real reaction_coefficient_derivative() const;
  Real electron_temperature(const Real E_N) const;
  // Real reduced_field(const Real reduced_field_old, const Real gas_density) const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  SplineInterpolation _coefficient_interpolation;
  Real _rate_constant;

  std::string _sampling_format;
  std::string _rate_format;
  const VariableValue & _reduced_field_value;
  // Function & _function;
  // const Point & _point;
  // Real _reaction_rate;
  // Real _d_k_d_en;
  // std::string _sampling_format;
};

