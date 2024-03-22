//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "HeatCapacityRatio.h"

#include "CraneUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", HeatCapacityRatio);

InputParameters
HeatCapacityRatio::validParams()
{
  InputParameters params = SpeciesSum::validParams();
  params += CraneUtils::propertyPathParams();
  params.addRequiredParam<std::vector<std::string>>("species", "The list of gaseous species contributing to gas temperature.");
  params.addParam<FileName>(
      "file_location",
      ".",
      "The name of the file that stores reaction rate tables (defaults to the current directory).");
  params.addCoupledVar("gas_temperature", "The temperature of the background gas. Needed for rate constant calculation. Default: 300 K.");
  // params.addCoupledVar("all_species", "The coupled variables to sum.");
  return params;
}

HeatCapacityRatio::HeatCapacityRatio(const InputParameters & parameters)
  : SpeciesSum(parameters),
  _gamma_heat(declareProperty<Real>("gamma_heat")),
  _species(getParam<std::vector<std::string>>("species")),
  _species_sum(getMaterialProperty<Real>("species_sum")),
  _Tgas(isCoupled("gas_temperature") ? coupledValue("gas_temperature") : _zero)
{
  // This class might be able to be inherited from SpeciesSum.
  // This is the same function until the computeQpProperties section.
  // int n = coupledComponents("coupled_vars");
  //
  // _vals.resize(n);
  //
  // for (unsigned int i=0; i<_vals.size(); ++i)
  // {
  //   _vals[i] = &coupledValue("coupled_vars", i);
  // }

  _polynomial_coefficients.resize(_species.size());
  _molar_heat_capacity.resize(_species.size());
  for (unsigned int i = 0; i < _species.size(); ++i)
    _polynomial_coefficients[i] = CraneUtils::getCoefficients(*this, _species[i] + ".txt");
}

void
HeatCapacityRatio::computeQpProperties()
{
  Real heat_frac = 0.0;
  // This loop (and the line immediately following the loop):
  // Finko, equations 16-17
  for (unsigned int i=0; i<_species.size(); ++i)
  {
    _molar_heat_capacity[i] = _polynomial_coefficients[i][0] + _polynomial_coefficients[i][1]*_Tgas[_qp] +
      _polynomial_coefficients[i][2]*std::pow(_Tgas[_qp], 2.0) + _polynomial_coefficients[i][3]*std::pow(_Tgas[_qp], 3.0) +
      _polynomial_coefficients[i][4]*std::pow(_Tgas[_qp], 4.0);
    heat_frac += (*_vals[i])[_qp] * _molar_heat_capacity[i];
  }
  heat_frac = heat_frac * (1.0/_species_sum[_qp]);

  _gamma_heat[_qp] = heat_frac / (heat_frac - 1.0);  // Finko, equation 15
}
