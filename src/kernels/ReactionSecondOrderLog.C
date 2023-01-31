//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReactionSecondOrderLog.h"

using MetaPhysicL::raw_value;

registerMooseObject("CraneApp", ReactionSecondOrderLog);
registerMooseObject("CraneApp", ADReactionSecondOrderLog);

template <bool is_ad>
InputParameters
ReactionSecondOrderLogTempl<is_ad>::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "If v == u.");
  params.addParam<bool>("_w_eq_u", false, "If w == u.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

template <bool is_ad>
ReactionSecondOrderLogTempl<is_ad>::ReactionSecondOrderLogTempl(const InputParameters & parameters)
  : GenericKernel<is_ad>(parameters),
    _v(this->template coupledGenericValue<is_ad>("v")),
    _w(this->template coupledGenericValue<is_ad>("w")),
    _v_id(coupled("v")),
    _w_id(coupled("w")),
    _reaction_coeff(this->template getGenericMaterialProperty<Real, is_ad>(
        "k" + this->template getParam<std::string>("number") + "_" +
        this->template getParam<std::string>("reaction"))),
    _stoichiometric_coeff(this->template getParam<Real>("coefficient")),
    _v_eq_u(this->template getParam<bool>("_v_eq_u")),
    _w_eq_u(this->template getParam<bool>("_w_eq_u"))
{
}

template <bool is_ad>
GenericReal<is_ad>
ReactionSecondOrderLogTempl<is_ad>::computeQpResidual()
{
  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp]);
}

template <bool is_ad>
Real
ReactionSecondOrderLogTempl<is_ad>::computeQpJacobian()
{
  Real power;
  power = 0.0;

  if (_v_eq_u)
    power += 1.0;

  if (_w_eq_u)
    power += 1.0;

  if (!_v_eq_u && !_w_eq_u)
  {
    return 0.0;
  }
  else
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * power *
           raw_value(_reaction_coeff[_qp] * std::exp(_v[_qp] + _w[_qp])) * _phi[_j][_qp];
  }
}

template <bool is_ad>
Real
ReactionSecondOrderLogTempl<is_ad>::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power;
  power = 0;

  if (!_v_eq_u && jvar == _v_id)
    power += 1;

  if (!_w_eq_u && jvar == _w_id)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff *
         raw_value(_reaction_coeff[_qp] * std::exp(_v[_qp] + _w[_qp])) * power * _phi[_j][_qp];
}

template class ReactionSecondOrderLogTempl<false>;
template class ReactionSecondOrderLogTempl<true>;
