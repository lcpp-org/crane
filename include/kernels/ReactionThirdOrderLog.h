#pragma once

#include "GenericKernel.h"

template <bool is_ad>
class ReactionThirdOrderLogTempl : public GenericKernel<is_ad>
{
public:
  ReactionThirdOrderLogTempl(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual GenericReal<is_ad> computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  const GenericVariableValue<is_ad> & _v;
  const GenericVariableValue<is_ad> & _w;
  const GenericVariableValue<is_ad> & _x;
  unsigned int _v_id;
  unsigned int _w_id;
  unsigned int _x_id;
  bool _v_eq_u;
  bool _w_eq_u;
  bool _x_eq_u;

  const GenericMaterialProperty<Real, is_ad> & _reaction_coeff;
  Real _stoichiometric_coeff;

  usingGenericKernelMembers;
};

typedef ReactionThirdOrderLogTempl<false> ReactionThirdOrderLog;
typedef ReactionThirdOrderLogTempl<true> ADReactionThirdOrderLog;
