//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EEDFRateConstantTownsend.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", EEDFRateConstantTownsend);

InputParameters
EEDFRateConstantTownsend::validParams()
{
  InputParameters params = Material::validParams();
  params.addRequiredParam<RelativeFileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<FileName>("file_location",
                                    "The name of the file that stores the reaction rate tables.");
  params.addParam<bool>("elastic_collision",
                        false,
                        "Determining whether or not a collision is elastic. Energy change for "
                        "elastic collisions is calculated on the fly, not pre-assigned.");
  params.addParam<bool>("is_target_aux",
                        false,
                        "Whether the coupled target species is an aux variable or not. (If it is, "
                        "it does not contribute to jacobian terms.)");
  params.addCoupledVar("target_species", "The heavy (target) species. Optional (default: _n_gas).");
  params.addRequiredCoupledVar("mean_energy", "The electron mean energy in log form.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

EEDFRateConstantTownsend::EEDFRateConstantTownsend(const InputParameters & parameters)
  : Material(parameters),
    _townsend_coefficient(declareProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                                getParam<std::string>("reaction"))),
    _d_alpha_d_en(declareProperty<Real>("d_alpha" + getParam<std::string>("number") + "_d_en_" +
                                        getParam<std::string>("reaction"))),
    _d_alpha_d_var_id(declareProperty<unsigned int>("d_alpha" + getParam<std::string>("number") +
                                                    "_d_var_id_" +
                                                    getParam<std::string>("reaction"))),
    _em(coupledValue("electrons")),
    _mean_en(coupledValue("mean_energy"))
{
  std::vector<Real> temp_x;
  std::vector<Real> temp_y;
  std::vector<Real> actual_mean_energy;
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
      temp_x.push_back(value);
      myfile >> value;
      temp_y.push_back(value);

      actual_mean_energy.push_back(1);
      rate_coefficient.push_back(1);
    }
    myfile.close();
  }
  else
    mooseError("Unable to open file");

  // Ensure that arrays are sorted (should be done externally or by Bolsig+ wrapper; this is not
  // permanent)
  std::vector<size_t> idx(actual_mean_energy.size());
  std::iota(idx.begin(), idx.end(), 0);
  std::sort(
      idx.begin(), idx.end(), [&temp_x](size_t i1, size_t i2) { return temp_x[i1] < temp_x[i2]; });
  for (MooseIndex(idx) i = 0; i < idx.size(); ++i)
  {
    actual_mean_energy[i] = temp_x[idx[i]];
    rate_coefficient[i] = temp_y[idx[i]];
  }
  _coefficient_interpolation.setData(actual_mean_energy, rate_coefficient);
}

void
EEDFRateConstantTownsend::computeQpProperties()
{
  Real actual_mean_energy = std::exp(_mean_en[_qp] - _em[_qp]);

  _townsend_coefficient[_qp] = _coefficient_interpolation.sample(actual_mean_energy);

  _d_alpha_d_en[_qp] = _coefficient_interpolation.sampleDerivative(actual_mean_energy);

  if (_townsend_coefficient[_qp] < 0)
  {
    _townsend_coefficient[_qp] = 0;
    _d_alpha_d_en[_qp] = 0;
  }
}
