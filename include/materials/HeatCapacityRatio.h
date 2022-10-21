#pragma once

// #include "Material.h"
#include "SpeciesSum.h"

class HeatCapacityRatio : public SpeciesSum
{
public:
  HeatCapacityRatio(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();
  MaterialProperty<Real> & _gamma_heat;
  const std::vector<std::string> & _species;
  const MaterialProperty<Real> & _species_sum;
  const VariableValue & _Tgas;
  std::vector<std::vector<Real>> _polynomial_coefficients;
  std::vector<Real> _molar_heat_capacity;

private:
  std::vector<const VariableValue *> _vals;

};
