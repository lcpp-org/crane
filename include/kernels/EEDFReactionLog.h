/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#pragma once

#include "Kernel.h"

class EEDFReactionLog;

template <>
InputParameters validParams<EEDFReactionLog>();

class EEDFReactionLog : public Kernel
{
public:
  EEDFReactionLog(const InputParameters & parameters);
  virtual ~EEDFReactionLog();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real _r_units;

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
