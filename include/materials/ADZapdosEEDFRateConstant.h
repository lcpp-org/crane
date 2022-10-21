#pragma once

#include "ADMaterial.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"
//#include "LinearInterpolation.h"

class ADZapdosEEDFRateConstant : public ADMaterial
{
public:
  static InputParameters validParams();
  ADZapdosEEDFRateConstant(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  //std::unique_ptr<LinearInterpolation> _coefficient_interpolation;
  SplineInterpolation _coefficient_interpolation;

  Real _r_units;
  ADMaterialProperty<Real> & _rate_coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;

  using ADMaterial::_communicator;
};
