#pragma once

#include "Kernel.h"

class ReactionSecondOrderEnergyLog : public Kernel
{
public:
  ReactionSecondOrderEnergyLog(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const VariableValue & _v;
  const VariableValue & _w;
  unsigned int _v_id;
  unsigned int _w_id;

  // The reaction coefficient
  const MaterialProperty<Real> & _reaction_coeff;
  Real _stoichiometric_coeff;
  Real _threshold_energy;
  bool _v_eq_u;
  bool _w_eq_u;
};
