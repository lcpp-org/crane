#pragma once

#include "ADMaterial.h"
#include "SplineInterpolation.h"

class InterpolatedCoefficientSpline : public ADMaterial
{
public:
  static InputParameters validParams();
  InterpolatedCoefficientSpline(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;

  ADMaterialProperty<Real> & _coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;
};
