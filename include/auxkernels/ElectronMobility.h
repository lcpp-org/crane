#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"

class ElectronMobility : public AuxScalarKernel
{
public:
  ElectronMobility(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~ElectronMobility();

protected:
  virtual Real computeValue();

  SplineInterpolation _mobility;

  const VariableValue & _reduced_field;
};
