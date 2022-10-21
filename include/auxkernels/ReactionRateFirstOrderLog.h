#pragma once

#include "AuxKernel.h"

class ReactionRateFirstOrderLog : public AuxKernel
{
public:
  ReactionRateFirstOrderLog(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateFirstOrderLog() {}
  virtual Real computeValue();

protected:


  const VariableValue & _v;
  const MaterialProperty<Real> & _reaction_coeff;
};
