#pragma once

#include "Material.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"

class ZapdosEEDFRateConstant : public Material
{
public:
  ZapdosEEDFRateConstant(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;

  MaterialProperty<Real> & _reaction_rate;
  MaterialProperty<Real> & _d_k_d_en;
  const VariableValue & _em;
  const VariableValue & _mean_en;
};
