#pragma once

#include "ADMaterial.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"
//#include "LinearInterpolation.h"

class ADEEDFRateConstantTownsend : public ADMaterial
{
public:
  static InputParameters validParams();
  ADEEDFRateConstantTownsend(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;
  
  std::string _coefficient_format;
  ADMaterialProperty<Real> & _townsend_coefficient;
  const MaterialProperty<Real> & _massIncident;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;

};
