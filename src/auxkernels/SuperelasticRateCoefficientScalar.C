//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SuperelasticRateCoefficientScalar.h"

registerMooseObject("CraneApp", SuperelasticRateCoefficientScalar);

InputParameters
SuperelasticRateCoefficientScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addRequiredCoupledVar("forward_coefficient", "The forward rate coefficient that is being reversed.");
  params.addCoupledVar("Tgas", 0, "The gas temperature in Kelvin (if it is a variable.).");
  params.addParam<Real>("Tgas_const", 0, "The gas temperature in Kelvin (if constant).");
  params.addRequiredParam<UserObjectName>("polynomial_provider",
      "The name of the UserObject that can provide the polynomial coefficients.");
  return params;
}

SuperelasticRateCoefficientScalar::SuperelasticRateCoefficientScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _forward_coefficient(coupledScalarValue("forward_coefficient")),
    _Tgas(coupledScalarValue("Tgas")),
    _Tgas_const(getParam<Real>("Tgas_const")),
    _polynomial(getUserObject<PolynomialCoefficients>("polynomial_provider"))
{
}

Real
SuperelasticRateCoefficientScalar::computeValue()
{
  Real equilibrium_constant;
  Real Tgas;
  if (isCoupledScalar("Tgas"))
    Tgas = _Tgas[_i];
  else
    Tgas = _Tgas_const;

  equilibrium_constant = std::pow(1.0, _polynomial.power_coefficient()) * std::exp(_polynomial.delta_a(0)*(std::log(Tgas - 1)) +
  (_polynomial.delta_a(1)/2.0)*Tgas + (_polynomial.delta_a(2)/6.0)*std::pow(Tgas, 2.0) + (_polynomial.delta_a(3)/12.0)*std::pow(Tgas, 3.0) +
  (_polynomial.delta_a(4)/20.0)*std::pow(Tgas, 4.0) - _polynomial.delta_a(5)*std::pow(Tgas, -1.0) + _polynomial.delta_a(6));

  return _forward_coefficient[_i] / equilibrium_constant;
}
