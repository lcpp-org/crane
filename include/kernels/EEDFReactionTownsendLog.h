#pragma once

#include "Kernel.h"

class EEDFReactionTownsendLog : public Kernel
{
public:
  EEDFReactionTownsendLog(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~EEDFReactionTownsendLog();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real _r_units;

  const MaterialProperty<Real> & _diffem;
  const MaterialProperty<Real> & _muem;
  const MaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _d_iz_d_actual_mean_en;
  const MaterialProperty<Real> & _d_muem_d_actual_mean_en;
  const MaterialProperty<Real> & _d_diffem_d_actual_mean_en;

  const VariableValue & _mean_en;
  const VariableGradient & _grad_potential;
  const VariableValue & _em;
  const VariableGradient & _grad_em;
  unsigned int _mean_en_id;
  unsigned int _potential_id;
  unsigned int _em_id;
  const VariableValue & _target;
  unsigned int _target_id;
  Real _coefficient;
};
