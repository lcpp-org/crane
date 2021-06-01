#pragma once

#include "ADTimeDerivative.h"

class TimeDerivativeLog : public ADTimeDerivative
{
public:
  static InputParameters validParams();

  TimeDerivativeLog(const InputParameters & parameters);

protected:
  virtual ADReal precomputeQpResidual() override;
};
