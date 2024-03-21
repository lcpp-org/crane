//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "DiffusionRateTemp.h"
#include "CraneUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", DiffusionRateTemp);

InputParameters
DiffusionRateTemp::validParams()
{
  InputParameters params = Material::validParams();
  params += CraneUtils::propertyFileParams();
  params.makeParamNotRequired("property_file");
  params.set<RelativeFileName>("property_file") = "electron_temperature.txt";
  params.addRequiredParam<FileName>("file_location",
                                    "The name of the file that stores the mobility table.");
  // params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  // params.addRequiredParam<Real>("", "The value of the reaction rate (constant).");

  return params;
}

DiffusionRateTemp::DiffusionRateTemp(const InputParameters & parameters)
  : Material(parameters),
    _diff_rate(declareProperty<Real>("diffusion_rate")),
    _gap_length(getMaterialProperty<Real>("gap_length")),
    _radius(getMaterialProperty<Real>("radius"))
{
  const auto [reduced_field, temperature] = CraneUtils::getReactionRates(*this);
  _elec_temp.setData(reduced_field, temperature);
}

void
DiffusionRateTemp::computeQpProperties()
{
  // Should be around ~5250 for current testing purposes
  // _reaction_rate[_qp] = _rate_value;
  // _diff_rate[_qp] = 1.52 * (760.0 / _pressure[_qp]) * (300.0 / 273.16) * (_elec_temp[_qp] / 11600) *
  //   ( std::pow(2.405/_radius,2.0) + std::pow(3.141/_gap_length,2.0) );

  _diff_rate[_qp] = 1.52 * (760.0 / 100.0) * (300.0 / 273.16) * (50946 / 11600) *
    ( std::pow(2.405/0.4,2.0) + std::pow(3.141/0.4,2.0) );
}
