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

class ADEEDFReactionTownsendLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFReactionTownsendLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  Real _r_units;

  const ADMaterialProperty<Real> & _diffem;
  const ADMaterialProperty<Real> & _muem;
  const ADMaterialProperty<Real> & _alpha;

  //const ADVariableValue & _mean_en;
  const ADVariableGradient & _grad_potential;
  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _coefficient;
  const ADVariableGradient & _grad_em;
};
