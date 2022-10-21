#pragma once

#include "AuxKernel.h"

class ReactionRateSecondOrder : public AuxKernel
{
public:
  ReactionRateSecondOrder(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateSecondOrder() {}
  virtual Real computeValue();

protected:
  const VariableValue & _v;
  const VariableValue & _w;
  const MaterialProperty<Real> & _reaction_coeff;
};

