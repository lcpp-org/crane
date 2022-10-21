#pragma once

#include "AuxKernel.h"

class ReactionRateThirdOrderLog : public AuxKernel
{
public:
  ReactionRateThirdOrderLog(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateThirdOrderLog() {}
  virtual Real computeValue();

protected:
  const VariableValue & _v;
  const VariableValue & _w;
  const VariableValue & _x;
  const MaterialProperty<Real> & _reaction_coeff;
};

