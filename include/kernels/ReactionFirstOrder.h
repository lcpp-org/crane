#pragma once

#include "Kernel.h"

class ReactionFirstOrder : public Kernel
{
public:
  ReactionFirstOrder(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  Real computeQpResidual() override;
  Real computeQpJacobian() override;
  Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const VariableValue & _v;
  unsigned int _v_id;

  // The reaction coefficient
  const MaterialProperty<Real> & _reaction_coeff;
  Real _stoichiometric_coeff;
  bool _v_eq_u;
};
