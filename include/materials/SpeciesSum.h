#pragma once

#include "Material.h"

class SpeciesSum : public Material
{
public:
  SpeciesSum(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  MaterialProperty<Real> & _total_sum;

private:
  std::vector<const VariableValue *> _vals;
  // std::vector<const VariableGradient *> _grad_vals;

};
