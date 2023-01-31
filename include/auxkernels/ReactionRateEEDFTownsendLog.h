//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
