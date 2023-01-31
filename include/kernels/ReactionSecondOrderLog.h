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

#include "GenericKernel.h"

template <bool is_ad>
class ReactionSecondOrderLogTempl : public GenericKernel<is_ad>
{
public:
  ReactionSecondOrderLogTempl(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual GenericReal<is_ad> computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const GenericVariableValue<is_ad> & _v;
  const GenericVariableValue<is_ad> & _w;
  unsigned int _v_id;
  unsigned int _w_id;

  // The reaction coefficient
  const GenericMaterialProperty<Real, is_ad> & _reaction_coeff;
  Real _stoichiometric_coeff;
  bool _v_eq_u;
  bool _w_eq_u;

  usingGenericKernelMembers;
};

typedef ReactionSecondOrderLogTempl<false> ReactionSecondOrderLog;
typedef ReactionSecondOrderLogTempl<true> ADReactionSecondOrderLog;
