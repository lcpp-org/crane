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

#include "ADKernel.h"

class ADEEDFEnergyLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFEnergyLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty<Real> & _reaction_coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _target;

  // Threshold energy is just a parameter generally, though elastic collisions require a material property.
  Real _threshold_energy;
};
