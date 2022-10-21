#pragma once

#include "AuxScalarKernel.h"

class ReactionRateTwoBodyScalar : public AuxScalarKernel
{
public:
  ReactionRateTwoBodyScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const VariableValue & _v;
  const VariableValue & _w;

  const VariableValue & _rate_coefficient;
  Real _stoichiometric_coeff;
};
