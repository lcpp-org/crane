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

#include "Kernel.h"

class EEDFReactionLog : public Kernel
{
public:
  EEDFReactionLog(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~EEDFReactionLog();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const MaterialProperty<Real> & _reaction_coeff;
  const MaterialProperty<Real> & _d_k_d_actual_mean_en;

  const VariableValue & _mean_en;
  const VariableValue & _em;
  unsigned int _mean_en_id;
  unsigned int _em_id;
  const VariableValue & _target;
  unsigned int _target_id;
  Real _coefficient;
};
