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

class ADEEDFElasticLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFElasticLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty<Real> & _reaction_coefficient;
  const MaterialProperty<Real> & _massGas;

  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _massem;
};
