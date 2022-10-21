#pragma once

#include "Kernel.h"

class LogStabilization : public Kernel
{
public:
  LogStabilization(const InputParameters & parameters);
  static InputParameters validParams();

  virtual ~LogStabilization();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _offset;
};
