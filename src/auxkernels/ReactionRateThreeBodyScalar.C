//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReactionRateThreeBodyScalar.h"

registerMooseObject("CraneApp", ReactionRateThreeBodyScalar);

InputParameters
ReactionRateThreeBodyScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredCoupledVar("z", "The second variable that is reacting to create u.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ReactionRateThreeBodyScalar::ReactionRateThreeBodyScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _v(coupledScalarValue("v")),
    _w(coupledScalarValue("w")),
    _z(coupledScalarValue("z")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactionRateThreeBodyScalar::computeValue()
{
  return _stoichiometric_coeff * _rate_coefficient[_i] * _v[_i] * _w[_i] * _z[_i];
}
