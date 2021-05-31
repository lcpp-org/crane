#pragma once

#include "Material.h"

template <bool is_ad>
class GenericRateConstantTempl : public Material
{
public:
  GenericRateConstantTempl(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties() override;

  GenericMaterialProperty<Real, is_ad> & _reaction_rate;
  MaterialProperty<Real> & _d_k_d_en;

  Real _rate_value;

};

typedef GenericRateConstantTempl<false> GenericRateConstant;
typedef GenericRateConstantTempl<true> ADGenericRateConstant;
