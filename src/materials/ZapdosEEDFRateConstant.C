//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ZapdosEEDFRateConstant.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ZapdosEEDFRateConstant);

InputParameters
ZapdosEEDFRateConstant::validParams()
{
  InputParameters params = Material::validParams();
  params.addRequiredParam<RelativeFileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<FileName>("file_location",
                                    "The name of the file that stores the reaction rate tables.");
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

ZapdosEEDFRateConstant::ZapdosEEDFRateConstant(const InputParameters & parameters)
  : Material(parameters),
    _reaction_rate(declareProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                         getParam<std::string>("reaction"))),
    _d_k_d_en(declareProperty<Real>("d_k" + getParam<std::string>("number") + "_d_en_" +
                                    getParam<std::string>("reaction"))),
    _em(isCoupled("electrons") ? coupledValue("electrons") : _zero),
    _mean_en(isCoupled("mean_energy") ? coupledValue("mean_energy") : _zero)
{
  if (!isParamValid("sampler") && !isParamValid("mean_energy"))
    mooseError("Material ZapdosEEDFRateConstant requires either a sampling variable or the "
               "electron and mean energy variables to be set!");
  std::vector<Real> val_x;
  std::vector<Real> rate_coefficient;
  std::string file_name =
      getParam<FileName>("file_location") + "/" + getParam<RelativeFileName>("property_file");
  MooseUtils::checkFileReadable(file_name);
  const char * charPath = file_name.c_str();
  std::ifstream myfile(charPath);
  Real value;

  if (myfile.is_open())
  {
    while (myfile >> value)
    {
      val_x.push_back(value);
      myfile >> value;
      rate_coefficient.push_back(value);
    }
    myfile.close();
  }
  else
    mooseError("Unable to open file");

  _coefficient_interpolation.setData(val_x, rate_coefficient);
}

void
ZapdosEEDFRateConstant::computeQpProperties()
{
  Real actual_mean_energy = std::exp(_mean_en[_qp] - _em[_qp]);

  _reaction_rate[_qp] = _coefficient_interpolation.sample(actual_mean_energy);

  _d_k_d_en[_qp] = _coefficient_interpolation.sampleDerivative(actual_mean_energy);

  if (_reaction_rate[_qp] < 0.0)
    _reaction_rate[_qp] = 0.0;
}
