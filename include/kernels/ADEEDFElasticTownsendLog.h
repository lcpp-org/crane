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

class ADEEDFElasticTownsendLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFElasticTownsendLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  Real _r_units;
  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty<Real> & _diffem;
  const ADMaterialProperty<Real> & _muem;
  const ADMaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _massGas;

  const ADVariableGradient & _grad_potential;
  const ADVariableValue & _em;
  const ADVariableValue & _target;
  const ADVariableGradient & _grad_em;
  Real _massem;
};
