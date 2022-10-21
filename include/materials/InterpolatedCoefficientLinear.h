#pragma once

#include "ADMaterial.h"
/* #include "LinearInterpolation.h" */
//#include "SplineInterpolation.h"
#include "LinearInterpolation.h"

class InterpolatedCoefficientLinear : public ADMaterial
{
public:
  static InputParameters validParams();
  InterpolatedCoefficientLinear(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  std::unique_ptr<LinearInterpolation> _coefficient_interpolation;

  ADMaterialProperty<Real> & _coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;
};
