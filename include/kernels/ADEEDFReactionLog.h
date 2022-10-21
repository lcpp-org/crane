#pragma once

#include "ADKernel.h"

class ADEEDFReactionLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFReactionLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  const ADMaterialProperty<Real> & _reaction_coeff;

  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _coefficient;
};
