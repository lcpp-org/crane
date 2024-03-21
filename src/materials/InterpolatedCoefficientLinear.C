//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "InterpolatedCoefficientLinear.h"

#include "CraneUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerADMooseObject("CraneApp", InterpolatedCoefficientLinear);

InputParameters
InterpolatedCoefficientLinear::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params += CraneUtils::propertyFileParams();
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredCoupledVar("mean_energy", "The electron mean energy in log form.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addParam<bool>("townsend",
                        false,
                        "Whether the tabulated data is a townsend coefficient or rate coefficient. "
                        "If true, the name of the Material property will begin with 'alpha'. If "
                        "false (default), it will begin with 'k'.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  params.addClassDescription(
      "Adds townsend coefficient for a reaction as a material property. Note that this material is "
      "only intended to be used with the plasma transport software, Zapdos. For any other MOOSE "
      "application (or when using Crane by itself) it is recommended to use rate coefficient "
      "formulation rather than the townsend coefficients, which may be done with the "
      "(AD)EEDFRateConstant material.");
  return params;
}

InterpolatedCoefficientLinear::InterpolatedCoefficientLinear(const InputParameters & parameters)
  : ADMaterial(parameters),
    _coefficient(declareADProperty<Real>(
        getParam<bool>("townsend")
            ? "alpha" + getParam<std::string>("number") + "_" + getParam<std::string>("reaction")
            : "k" + getParam<std::string>("number") + "_" + getParam<std::string>("reaction"))),
    _em(adCoupledValue("electrons")),
    _mean_en(adCoupledValue("mean_energy"))
{
  const auto [val_x, rate_coefficient] = CraneUtils::getReactionRates(*this);
  _coefficient_interpolation =
      std::make_unique<LinearInterpolation>(val_x, rate_coefficient, true);
}

void
InterpolatedCoefficientLinear::computeQpProperties()
{
  _coefficient[_qp].value() =
      _coefficient_interpolation->sample(std::exp(_mean_en[_qp].value() - _em[_qp].value()));
  _coefficient[_qp].derivatives() = _coefficient_interpolation->sampleDerivative(
                                        std::exp(_mean_en[_qp].value() - _em[_qp].value())) *
                                    std::exp(_mean_en[_qp].value() - _em[_qp].value()) *
                                    (_mean_en[_qp].derivatives() - _em[_qp].derivatives());

  // Safeguard agains zero values resulting from extrapolation
  if (_coefficient[_qp].value() < 0)
  {
    _coefficient[_qp].value() = 0;
    _coefficient[_qp].derivatives() = 0;
  }
}
