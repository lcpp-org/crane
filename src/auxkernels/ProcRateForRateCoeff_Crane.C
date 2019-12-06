//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ProcRateForRateCoeff_Crane.h"

registerMooseObject("ZapdosApp", ProcRateForRateCoeff_Crane);

template <>
InputParameters
validParams<ProcRateForRateCoeff_Crane>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addClassDescription(
    "Reaction rate for two body collisions in units of #/m^3s. User can pass"
    "choice of elastic, excitation, or ionization reaction rate coefficients");

  return params;
}

ProcRateForRateCoeff_Crane::ProcRateForRateCoeff_Crane(const InputParameters & parameters)
  : AuxKernel(parameters),

  _v(coupledValue("v")),
  _w(coupledValue("w")),
  _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction")))
{
}

Real
ProcRateForRateCoeff_Crane::computeValue()
{

  return 6.02e23 * _reaction_coeff[_qp] * std::exp(_v[_qp]) * std::exp(_w[_qp]);

}
