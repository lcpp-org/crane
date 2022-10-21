#pragma once

#include "ADKernel.h"

class ADEEDFElasticTownsendLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFElasticTownsendLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  Real _r_units;
  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty<Real> & _diffem;
  const ADMaterialProperty<Real> & _muem;
  const ADMaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _massGas;

  const ADVariableGradient & _grad_potential;
  const ADVariableValue & _em;
  const ADVariableValue & _target;
  const ADVariableGradient & _grad_em;
  Real _massem;
};
