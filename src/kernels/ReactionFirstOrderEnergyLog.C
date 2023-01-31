//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReactionFirstOrderEnergyLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactionFirstOrderEnergyLog);

InputParameters
ReactionFirstOrderEnergyLog::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("threshold_energy", "The change in enthalpy associated with this reaction.");
  params.addParam<bool>("_v_eq_u", false, "If v == u.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ReactionFirstOrderEnergyLog::ReactionFirstOrderEnergyLog(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledValue("v")),
    _v_id(coupled("v")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _v_eq_u(getParam<bool>("_v_eq_u"))
{
}

Real
ReactionFirstOrderEnergyLog::computeQpResidual()
{
  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) * _threshold_energy;
}

Real
ReactionFirstOrderEnergyLog::computeQpJacobian()
{
  return 0.0;
  /*
  Real power;
  power = 0.0;

  if (_v_eq_u)
  {
    power += 1.0;
  }

  if (!_v_eq_u)
  {
    return 0.0;
  }
  else
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * power * _reaction_coeff[_qp] *
           std::exp(_v[_qp]) * _phi[_j][_qp];
  }
  */
}

Real
ReactionFirstOrderEnergyLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (!_v_eq_u && jvar == _v_id)
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * std::exp(_v[_qp]) *
           _phi[_j][_qp] * _threshold_energy;
  }
  else
  {
    return 0.0;
  }
}
