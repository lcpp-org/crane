#pragma once

#include "AuxScalarKernel.h"
// #include "SplineInterpolation.h"

class MoleFraction : public AuxScalarKernel
{
public:
  MoleFraction(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();
  const VariableValue & _neutral_density;
  const VariableValue & _species_density;
};
