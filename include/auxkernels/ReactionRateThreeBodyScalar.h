#pragma once

#include "AuxScalarKernel.h"

class ReactionRateThreeBodyScalar : public AuxScalarKernel
{
public:
  ReactionRateThreeBodyScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const VariableValue & _v;
  const VariableValue & _w;
  const VariableValue & _z;

  const VariableValue & _rate_coefficient;
  Real _stoichiometric_coeff;
};
