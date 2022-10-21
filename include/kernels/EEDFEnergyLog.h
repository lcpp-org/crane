#pragma once

#include "Kernel.h"

class EEDFEnergyLog : public Kernel
{
public:
  EEDFEnergyLog(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~EEDFEnergyLog();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  Real _threshold_energy;
  const MaterialProperty<Real> & _reaction_coeff;
  const MaterialProperty<Real> & _d_k_d_actual_mean_en;

  const VariableValue & _em;
  unsigned int _em_id;
  const VariableValue & _target;
  unsigned int _target_id;
};
