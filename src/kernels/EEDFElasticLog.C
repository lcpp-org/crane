//* This file is part of CRANE, an open-source
//* application for the simulation of plasmas
//* https://github.com/lcpp-org/crane
//*
//* CRANE is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EEDFElasticLog.h"

registerMooseObject("CraneApp", EEDFElasticLog);

template <>
InputParameters
validParams<EEDFElasticLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("electron_species", "The electron density.");
  params.addRequiredCoupledVar("target_species", "The target species variable.");
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

EEDFElasticLog::EEDFElasticLog(const InputParameters & parameters)
  : Kernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _massTarget(getMaterialProperty<Real>("mass" + (*getVar("target_species", 0)).name())),
    _d_k_d_actual_mean_en(getMaterialProperty<Real>("d_k" + getParam<std::string>("number") +
                                                    "_d_en_" + getParam<std::string>("reaction"))),
    _em(coupledValue("electron_species")),
    _em_id(coupled("electron_species")),
    _target(coupledValue("target_species")),
    _target_id(coupled("target_species"))
{
  _massem = 9.11e-31;
}

EEDFElasticLog::~EEDFElasticLog() {}

Real
EEDFElasticLog::computeQpResidual()
{
  Real Eel = -3.0 * _massem / _massTarget[_qp] * 2.0 / 3 * std::exp(_u[_qp] - _em[_qp]);

  return -_test[_i][_qp] * _reaction_coeff[_qp] * std::exp(_em[_qp] + _target[_qp]) * Eel;
}

Real
EEDFElasticLog::computeQpJacobian()
{
  Real d_actual_mean_en_d_mean_en = std::exp(_u[_qp] - _em[_qp]) * _phi[_j][_qp];
  Real d_k_d_mean_en = _d_k_d_actual_mean_en[_qp] * d_actual_mean_en_d_mean_en;

  Real Eel = -3.0 * _massem / _massTarget[_qp] * 2.0 / 3 * std::exp(_u[_qp] - _em[_qp]);
  Real d_Eel_d_mean_en = Eel * _phi[_j][_qp];

  return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) *
         (d_k_d_mean_en * Eel + _reaction_coeff[_qp] * d_Eel_d_mean_en);
}

Real
EEDFElasticLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real d_actual_mean_en_d_em = -std::exp(_u[_qp] - _em[_qp]) * _phi[_j][_qp];
  Real d_k_d_em = _d_k_d_actual_mean_en[_qp] * d_actual_mean_en_d_em;

  Real Eel = -3.0 * _massem / _massTarget[_qp] * 2.0 / 3 * std::exp(_u[_qp] - _em[_qp]);
  Real d_Eel_d_em =
      -3.0 * _massem / _massTarget[_qp] * 2.0 / 3 * std::exp(_u[_qp] - _em[_qp]) * -_phi[_j][_qp];

  if (jvar == _em_id)
    return -_test[_i][_qp] *
           (d_k_d_em * std::exp(_em[_qp] + _target[_qp]) * Eel +
            _reaction_coeff[_qp] * std::exp(_em[_qp] + _target[_qp]) * _phi[_j][_qp] * Eel +
            _reaction_coeff[_qp] * std::exp(_em[_qp] + _target[_qp]) * d_Eel_d_em);
  else if (jvar == _target_id)
    return -_test[_i][_qp] * _reaction_coeff[_qp] * Eel * std::exp(_em[_qp] + _target[_qp]) *
           _phi[_j][_qp];
  else
    return 0.0;
}
