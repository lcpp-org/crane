#pragma once

#include "AuxKernel.h"

class ReactionRateFirstOrder : public AuxKernel
{
public:
  ReactionRateFirstOrder(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateFirstOrder() {}
  virtual Real computeValue();

protected:


  const VariableValue & _v;
  const MaterialProperty<Real> & _reaction_coeff;
};
