//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ADEEDFEnergyLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFEnergyLog);

InputParameters
ADEEDFEnergyLog::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredCoupledVar("target", "The target species.");
  params.addRequiredParam<Real>("threshold_energy", "Energy required for reaction to take place.");
  params.addRequiredParam<std::string>("reaction", "Stores the full reaction equation.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ADEEDFEnergyLog::ADEEDFEnergyLog(const InputParameters & parameters)
  : ADKernel(parameters),
    _reaction_name(getParam<std::string>("reaction")),
    _reaction_coefficient(getADMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                                      getParam<std::string>("reaction"))),
    _em(adCoupledValue("electrons")),
    _target(adCoupledValue("target")),
    _threshold_energy(getParam<Real>("threshold_energy"))
{
}

ADReal
ADEEDFEnergyLog::computeQpResidual()
{
  return -_test[_i][_qp] * _reaction_coefficient[_qp] * std::exp(_em[_qp] + _target[_qp]) *
         _threshold_energy;
}
