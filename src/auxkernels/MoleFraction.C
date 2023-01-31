//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MoleFraction.h"

registerMooseObject("CraneApp", MoleFraction);

InputParameters
MoleFraction::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addCoupledVar("neutral_density", "The neutral gas density (calculated by VariableSum).");
  params.addCoupledVar("species_density", "The species density (nonlinear variable).");
  return params;
}

MoleFraction::MoleFraction(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _neutral_density(coupledScalarValue("neutral_density")),
    _species_density(coupledScalarValue("species_density"))
{
}

Real
MoleFraction::computeValue()
{
  return _species_density[_i] / _neutral_density[_i];
}
