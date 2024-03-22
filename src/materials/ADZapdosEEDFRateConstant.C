//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADZapdosEEDFRateConstant.h"

#include "CraneUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADZapdosEEDFRateConstant);

InputParameters
ADZapdosEEDFRateConstant::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params += CraneUtils::propertyFileParams();
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addCoupledVar("mean_energy", "The electron mean energy in log form.");
  params.addCoupledVar("electrons", "The electron density.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ADZapdosEEDFRateConstant::ADZapdosEEDFRateConstant(const InputParameters & parameters)
  : ADMaterial(parameters),
    _rate_coefficient(declareADProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _em(adCoupledValue("electrons")),
    _mean_en(isCoupled("mean_energy") ? adCoupledValue("mean_energy") : _em)
{
  const auto [val_x, rate_coefficient] = CraneUtils::getReactionRates(*this);
  _coefficient_interpolation.setData(val_x, rate_coefficient);
}

void
ADZapdosEEDFRateConstant::computeQpProperties()
{
  _rate_coefficient[_qp].value() =
      _coefficient_interpolation.sample(std::exp(_mean_en[_qp].value() - _em[_qp].value()));
  _rate_coefficient[_qp].derivatives() = _coefficient_interpolation.sampleDerivative(
                                             std::exp(_mean_en[_qp].value() - _em[_qp].value())) *
                                         std::exp(_mean_en[_qp].value() - _em[_qp].value()) *
                                         (_mean_en[_qp].derivatives() - _em[_qp].derivatives());

  if (_rate_coefficient[_qp].value() < 0.0)
  {
    _rate_coefficient[_qp].value() = 0.0;
    _rate_coefficient[_qp].derivatives() = 0.0;
  }
}
