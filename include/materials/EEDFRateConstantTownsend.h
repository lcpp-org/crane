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

#include "Material.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"

class EEDFRateConstantTownsend : public Material
{
public:
  EEDFRateConstantTownsend(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;

  MaterialProperty<Real> & _townsend_coefficient;
  MaterialProperty<Real> & _d_alpha_d_en;
  MaterialProperty<unsigned int> & _d_alpha_d_var_id;

  const VariableValue & _em;
  const VariableValue & _mean_en;
};
