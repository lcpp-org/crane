#pragma once

#include "AuxScalarKernel.h"

class ReactionRateOneBodyScalar : public AuxScalarKernel
{
public:
  ReactionRateOneBodyScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const VariableValue & _v;

  // The reaction coefficient
  const VariableValue & _rate_coefficient;
  Real _stoichiometric_coeff;
};
