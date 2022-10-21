#pragma once

#include "AuxKernel.h"


template <bool is_ad>
class ReactionRateEEDFTownsendLogTempl : public AuxKernel
{
public:
  ReactionRateEEDFTownsendLogTempl(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ReactionRateEEDFTownsendLogTempl() {}
  virtual Real computeValue();

protected:
  Real _r_units;
  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const GenericMaterialProperty<Real, is_ad> & _diffem;
  const GenericMaterialProperty<Real, is_ad> & _muem;
  const GenericMaterialProperty<Real, is_ad> & _alpha;

  const VariableValue & _mean_en;
  const VariableGradient & _grad_potential;
  const VariableValue & _em;
  const VariableGradient & _grad_em;
  const VariableValue & _target;
};

typedef ReactionRateEEDFTownsendLogTempl<false> ReactionRateEEDFTownsendLog;
typedef ReactionRateEEDFTownsendLogTempl<true> ADReactionRateEEDFTownsendLog;
