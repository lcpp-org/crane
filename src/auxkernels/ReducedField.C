//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReducedField.h"
#include "MooseUtils.h"

registerMooseObject("CraneApp", ReducedField);

InputParameters
ReducedField::validParams()
{
 InputParameters params = AuxScalarKernel::validParams();
 params.addCoupledVar("electron_mobility", "The electron mobility.");
 params.addCoupledVar("electron_density", "The electron density.");
 params.addCoupledVar("gas_density", "The gas density.");
 return params;
}

ReducedField::ReducedField(const InputParameters & parameters)
 : AuxScalarKernel(parameters),
  _mobility(coupledScalarValue("electron_mobility"))
{
}

ReducedField::~ReducedField() {}

Real
ReducedField::computeValue()
{
  return 1.0;
 // return _mobility.sample(_reduced_field[_i]);
}
