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

class ADEEDFReactionLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFReactionLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  const ADMaterialProperty<Real> & _reaction_coeff;

  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _coefficient;
};
