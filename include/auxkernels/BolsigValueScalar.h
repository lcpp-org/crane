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

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"
#include "BoltzmannSolverScalar.h"

class BolsigValueScalar : public AuxScalarKernel
{
public:
  BolsigValueScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const BoltzmannSolverScalar & _data;
  std::string _data_type;
  bool _sample_value;
  const VariableValue & _sampler_var;
};
