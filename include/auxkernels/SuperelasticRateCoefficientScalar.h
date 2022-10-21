#pragma once

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"
#include "PolynomialCoefficients.h"

class SuperelasticRateCoefficientScalar : public AuxScalarKernel
{
public:
  SuperelasticRateCoefficientScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue();

  const VariableValue & _forward_coefficient;
  const VariableValue & _Tgas;
  Real _Tgas_const;
  const PolynomialCoefficients & _polynomial;
};
