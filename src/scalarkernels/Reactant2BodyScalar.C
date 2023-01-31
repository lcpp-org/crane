//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Reactant2BodyScalar.h"

registerMooseObject("CraneApp", Reactant2BodyScalar);

InputParameters
Reactant2BodyScalar::validParams()
{
  InputParameters params = ODEKernel::validParams();
  params.addRequiredCoupledVar("v", "Coupled variable 1.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  params.addParam<bool>("v_eq_u", false, "Whether or not u = v.");
  params.addParam<bool>(
      "rate_constant_equation", false, "True if rate constant is provided by equation.");
  return params;
}

Reactant2BodyScalar::Reactant2BodyScalar(const InputParameters & parameters)
  : ODEKernel(parameters),
    _v_var(isCoupledScalar("v") ? coupledScalar("v") : -1),
    _v(coupledScalarValue("v")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("v_eq_u")),
    _rate_constant_equation(getParam<bool>("rate_constant_equation"))
{
}

Real
Reactant2BodyScalar::computeQpResidual()
{
  return -_stoichiometric_coeff * _rate_coefficient[_i] * _u[_i] * _v[_i];
}

Real
Reactant2BodyScalar::computeQpJacobian()
{

  if (_v_eq_u)
  {
    return -_stoichiometric_coeff * _rate_coefficient[_i] * 2.0 * _u[_i];
  }
  else
    return -_stoichiometric_coeff * _rate_coefficient[_i] * _v[_i];
}

Real
Reactant2BodyScalar::computeQpOffDiagJacobian(unsigned int jvar)
{

  if (jvar == _v_var && !_v_eq_u)
  {
    return -_stoichiometric_coeff * _rate_coefficient[_i] * _u[_i];
  }
  else
    return 0.0;
}
