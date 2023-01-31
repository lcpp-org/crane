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

#include "ADMaterial.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"
//#include "LinearInterpolation.h"

class ADEEDFRateConstantTownsend : public ADMaterial
{
public:
  static InputParameters validParams();
  ADEEDFRateConstantTownsend(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;
  
  std::string _coefficient_format;
  ADMaterialProperty<Real> & _townsend_coefficient;
  const MaterialProperty<Real> & _massIncident;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;

};
