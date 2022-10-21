#pragma once

#include "Material.h"
#include "SplineInterpolation.h"

class DiffusionRateTemp : public Material
{
public:
  DiffusionRateTemp(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  SplineInterpolation _elec_temp;

  MaterialProperty<Real> & _diff_rate;
  const MaterialProperty<Real> & _gap_length;
  const MaterialProperty<Real> & _radius;
};
