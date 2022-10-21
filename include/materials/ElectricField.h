#pragma once

#include "Material.h"
#include "SplineInterpolation.h"

class ElectricField : public Material
{
public:
  ElectricField(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  SplineInterpolation _mobility;

  MaterialProperty<Real> & _reduced_field;
  const MaterialProperty<Real> & _voltage;
  const MaterialProperty<Real> & _gap_length;
  const MaterialProperty<Real> & _resistance;
  const MaterialProperty<Real> & _gap_area;
  MaterialProperty<Real> & _Vdr;
  const VariableValue & _electron_density;
  const VariableValue & _gas_density;
  const MaterialProperty<Real> & _reduced_field_old;
  bool _use_log;
  const MaterialProperty<Real> & _n_gas;

};
