#pragma once

#include "Kernel.h"

class ReactionFirstOrderEnergyLog : public Kernel
{
public:
  ReactionFirstOrderEnergyLog(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const VariableValue & _v;
  unsigned int _v_id;
  // const MaterialProperty<Real> & _n_gas;

  // The reaction coefficient
  const MaterialProperty<Real> & _reaction_coeff;
  Real _stoichiometric_coeff;
  Real _threshold_energy;
  bool _v_eq_u;
};
