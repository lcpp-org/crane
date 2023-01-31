//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReducedFieldScalar.h"

registerMooseObject("CraneApp", ReducedFieldScalar);

InputParameters
ReducedFieldScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addCoupledVar("electron_density", "The electron density.");
  params.addCoupledVar("gas_density", "The gas density.");
  // params.addRequiredParam<UserObjectName>("electron_temperature",
          // "The name of the UserObject that can provide the rate coefficient.");
  return params;
}

ReducedFieldScalar::ReducedFieldScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _electron_density(coupledScalarValue("electron_density")),
    _gas_density(coupledScalarValue("gas_density"))
    // _data(getUserObject<ValueProvider>("electron_temperature"))
{
}

ReducedFieldScalar::~ReducedFieldScalar() {}

Real
ReducedFieldScalar::computeValue()
{
  Real Voltage = 1000;
  Real gap_length = 0.4;
  Real resistance = 1e5;
  Real J = 1.0; // current calculation - needs to be calculated
  Real old_value = 1.0; // PLACEHOLDER - need to find _u_old[_i]

  return Voltage / (gap_length + resistance * J / (old_value * _gas_density[_i]/1.0e17) ) / _gas_density[_i]*1.0e17;
}
