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
#include "SplineInterpolation.h"

class DiffusionRateTemp : public Material
{
public:
  DiffusionRateTemp(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  SplineInterpolation _elec_temp;

  MaterialProperty<Real> & _diff_rate;
  const MaterialProperty<Real> & _gap_length;
  const MaterialProperty<Real> & _radius;
};
