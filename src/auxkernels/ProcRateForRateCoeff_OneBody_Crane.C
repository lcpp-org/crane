//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ProcRateForRateCoeff_OneBody_Crane.h"

registerMooseObject("ZapdosApp", ProcRateForRateCoeff_OneBody_Crane);

template <>
InputParameters
validParams<ProcRateForRateCoeff_OneBody_Crane>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");

  return params;
}

ProcRateForRateCoeff_OneBody_Crane::ProcRateForRateCoeff_OneBody_Crane(const InputParameters & parameters)
  : AuxKernel(parameters),

  _v(coupledValue("v")),
  _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction")))
{
}

Real
ProcRateForRateCoeff_OneBody_Crane::computeValue()
{

  return 6.02e23 * _reaction_coeff[_qp] * std::exp(_v[_qp]);

}
