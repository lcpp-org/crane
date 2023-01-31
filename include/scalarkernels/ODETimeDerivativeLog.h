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

#include "ODETimeKernel.h"

class ODETimeDerivativeLog : public ODETimeKernel
{
public:
  ODETimeDerivativeLog(const InputParameters & parameters);

  static InputParameters validParams();

  /* virtual void computeJacobian(); */

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  /* virtual Real computeQpOffDiagJacobian(unsigned int jvar); */

  bool _lumping;
};
