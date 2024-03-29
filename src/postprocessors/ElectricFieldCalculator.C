//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ElectricFieldCalculator.h"

// MOOSE includes
#include "MooseVariableScalar.h"
#include "Function.h"

registerMooseObject("CraneApp", ElectricFieldCalculator);

InputParameters
ElectricFieldCalculator::validParams()
{
  InputParameters params = GeneralPostprocessor::validParams();
  params.addParam<FunctionName>("function", 1.0,
    "This postprocessor will calculate this function using the coupled variables.");
  params.addCoupledVar("gas_density", "The neutral gas density.");
  params.addCoupledVar("electron_density", "The density of electrons.");
  return params;
}

ElectricFieldCalculator::ElectricFieldCalculator(const InputParameters & parameters)
  : GeneralPostprocessor(parameters),
    _gas_density(coupledScalarValue("gas_density")),
    _electron_density(coupledScalarValue("electron_density"))
{
}

Real
ElectricFieldCalculator::getValue() const
{
  return 1000;
  // _Vdr[_qp] = mult1 * _reduced_field_old[_qp] * _mobility.sample(_reduced_field_old[_qp]);
  // Real current = 1.602e-19 * _gap_area[_qp] * _electron_density[_qp] * _Vdr[_qp];
  //
  // _reduced_field[_qp] = _voltage[_qp] / ( _gap_length[_qp] + _resistance[_qp] * current /
    // ( _reduced_field_old[_qp]*mult1 ) ) / mult1;

  // reduced_field = 1000.0 / ( 0.4 + 1.0e5*1e-3/(reduced_field))
}
