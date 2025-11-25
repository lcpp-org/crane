//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADEEDFEnergyTownsendLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFEnergyTownsendLog);

InputParameters
ADEEDFEnergyTownsendLog::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredParam<std::string>("reaction", "The reaction that is adding/removing energy.");
  params.addRequiredParam<Real>("threshold_energy", "Energy required for reaction to take place.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addCoupledVar("target",
                       "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addClassDescription("Adds the change in enthalpy from a chemical reaction to the electron "
                             "and/or gas temperature equations.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ADEEDFEnergyTownsendLog::ADEEDFEnergyTownsendLog(const InputParameters & parameters)
  : ADKernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                       getParam<std::string>("reaction"))),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("electrons")),
    _grad_em(adCoupledGradient("electrons")),
    _target(adCoupledValue("target"))
{
}

ADReal
ADEEDFEnergyTownsendLog::computeQpResidual()
{
  using std::exp;
  _electron_flux_mag = (exp(_em[_qp]) * (-_muem[_qp] * -_grad_potential[_qp] * _r_units -
                                         _diffem[_qp] * _grad_em[_qp] * _r_units))
                           .norm();

  return -_test[_i][_qp] * _alpha[_qp] * exp(_target[_qp]) * _electron_flux_mag * _threshold_energy;
}
