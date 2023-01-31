//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "GenericRateConstant.h"

registerMooseObject("CraneApp", GenericRateConstant);
registerMooseObject("CraneApp", ADGenericRateConstant);

template <bool is_ad>
InputParameters
GenericRateConstantTempl<is_ad>::validParams()
{
  InputParameters params = Material::validParams();
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("reaction_rate_value",
                                "The value of the reaction rate (constant).");
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
GenericRateConstantTempl<is_ad>::GenericRateConstantTempl(const InputParameters & parameters)
  : Material(parameters),
    _reaction_rate(declareGenericProperty<Real, is_ad>("k" + getParam<std::string>("number") + "_" +
                                                       getParam<std::string>("reaction"))),
    _d_k_d_en(declareProperty<Real>("d_k" + getParam<std::string>("number") + "_d_en_" +
                                    getParam<std::string>("reaction"))),
    _rate_value(getParam<Real>("reaction_rate_value"))
{
}

template <>
void
GenericRateConstantTempl<false>::computeQpProperties()
{
  _reaction_rate[_qp] = _rate_value;
  _d_k_d_en[_qp] = 0.0;
}

template <>
void
GenericRateConstantTempl<true>::computeQpProperties()
{
  _reaction_rate[_qp].value() = _rate_value;
  _reaction_rate[_qp].derivatives() = 0.0;
}

template class GenericRateConstantTempl<false>;
template class GenericRateConstantTempl<true>;
