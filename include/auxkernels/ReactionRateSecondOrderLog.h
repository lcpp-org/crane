#pragma once

#include "AuxKernel.h"

class ReactionRateSecondOrderLog : public AuxKernel
{
public:
  ReactionRateSecondOrderLog(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateSecondOrderLog() {}
  virtual Real computeValue();

protected:
  const VariableValue & _v;
  const VariableValue & _w;
  const MaterialProperty<Real> & _reaction_coeff;
};

