#pragma once

#include "ADKernel.h"

class ADEEDFElasticLog : public ADKernel
{
public:
  static InputParameters validParams();
  ADEEDFElasticLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();

  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty<Real> & _reaction_coefficient;
  const MaterialProperty<Real> & _massGas;

  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _massem;
};
