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

#include "ADTimeDerivative.h"

class TimeDerivativeLog : public ADTimeDerivative
{
public:
  static InputParameters validParams();

  TimeDerivativeLog(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;
};
