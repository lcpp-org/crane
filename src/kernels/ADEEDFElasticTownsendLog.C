//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADEEDFElasticTownsendLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFElasticTownsendLog);

InputParameters
ADEEDFElasticTownsendLog::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredCoupledVar("target", "The target species variable.");
  params.addRequiredParam<std::string>("reaction",
                                       "Stores the name of the reaction (townsend) coefficient, "
                                       "unique to each individual reaction.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ADEEDFElasticTownsendLog::ADEEDFElasticTownsendLog(const InputParameters & parameters)
  : ADKernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                       getParam<std::string>("reaction"))),
    _massGas(getMaterialProperty<Real>("mass" + (*getVar("target", 0)).name())),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("electrons")),
    _target(adCoupledValue("target")),
    _grad_em(adCoupledGradient("electrons"))
{
  _massem = 9.11e-31;
}

ADReal
ADEEDFElasticTownsendLog::computeQpResidual()
{
  ADReal electron_flux_mag = (std::exp(_em[_qp]) * (-_muem[_qp] * -_grad_potential[_qp] * _r_units -
                                                    _diffem[_qp] * _grad_em[_qp] * _r_units))
                                 .norm();
  ADReal Eel = -3.0 * _massem / _massGas[_qp] * 2.0 / 3. * std::exp(_u[_qp] - _em[_qp]);
  ADReal el_term = _alpha[_qp] * std::exp(_target[_qp]) * electron_flux_mag * Eel;

  return -_test[_i][_qp] * el_term;
}
