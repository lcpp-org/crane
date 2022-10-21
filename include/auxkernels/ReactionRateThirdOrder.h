#pragma once

#include "AuxKernel.h"

class ReactionRateThirdOrder : public AuxKernel
{
public:
  ReactionRateThirdOrder(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateThirdOrder() {}
  virtual Real computeValue();

protected:
  const VariableValue & _v;
  const VariableValue & _w;
  const VariableValue & _x;
  const MaterialProperty<Real> & _reaction_coeff;
};

