//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Reactant1BodyScalar.h"

registerMooseObject("CraneApp", Reactant1BodyScalar);

InputParameters
Reactant1BodyScalar::validParams()
{
  InputParameters params = ODEKernel::validParams();
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  params.addParam<bool>(
      "rate_constant_equation", false, "True if rate constant is provided by equation.");
  return params;
}

Reactant1BodyScalar::Reactant1BodyScalar(const InputParameters & parameters)
  : ODEKernel(parameters),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
{
}

Real
Reactant1BodyScalar::computeQpResidual()
{
    return -_stoichiometric_coeff * _rate_coefficient[_i] * _u[_i];
}

Real
Reactant1BodyScalar::computeQpJacobian()
{
    return -_stoichiometric_coeff * _rate_coefficient[_i];
}
