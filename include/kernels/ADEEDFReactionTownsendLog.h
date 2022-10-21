#pragma once

#include "ADKernel.h"

class ADEEDFReactionTownsendLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFReactionTownsendLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  Real _r_units;

  const ADMaterialProperty<Real> & _diffem;
  const ADMaterialProperty<Real> & _muem;
  const ADMaterialProperty<Real> & _alpha;

  //const ADVariableValue & _mean_en;
  const ADVariableGradient & _grad_potential;
  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _coefficient;
  const ADVariableGradient & _grad_em;
};
