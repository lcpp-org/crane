#pragma once

#include "ODEKernel.h"

class LogStabilizationScalar : public ODEKernel
{
public:
  LogStabilizationScalar(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~LogStabilizationScalar();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _offset;
};
