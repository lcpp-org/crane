#include "EEDFEnergyLog.h"

registerMooseObject("CraneApp", EEDFEnergyLog);

template <>
InputParameters
validParams<EEDFEnergyLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("electron_species", "The electron density.");
  params.addRequiredParam<std::string>("reaction", "The reaction that is adding/removing energy.");
  params.addRequiredParam<Real>("threshold_energy", "Energy required for reaction to take place.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addCoupledVar("target_species",
                       "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

EEDFEnergyLog::EEDFEnergyLog(const InputParameters & parameters)
  : Kernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                     getParam<std::string>("reaction"))),
    _d_k_d_actual_mean_en(getMaterialProperty<Real>("d_k" + getParam<std::string>("number") +
                                                     "_d_en_" + getParam<std::string>("reaction"))),
    _em(coupledValue("electron_species")),
    _em_id(coupled("electron_species")),
    _target(coupledValue("target_species")),
    _target_id(coupled("target_species"))
{
}

EEDFEnergyLog::~EEDFEnergyLog() {}

Real
EEDFEnergyLog::computeQpResidual()
{
  return -_test[_i][_qp] * _reaction_coeff[_qp] * std::exp(_em[_qp] + _target[_qp]) * _threshold_energy;
}

Real
EEDFEnergyLog::computeQpJacobian()
{
  Real d_actual_mean_en_d_mean_en = std::exp(_u[_qp] - _em[_qp]) * _phi[_j][_qp];
  Real d_k_d_mean_en = _d_k_d_actual_mean_en[_qp] * d_actual_mean_en_d_mean_en;

  return -_test[_i][_qp] * d_k_d_mean_en * std::exp(_em[_qp] + _target[_qp]) * _threshold_energy;
}

Real
EEDFEnergyLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real d_actual_mean_en_d_em = -std::exp(_u[_qp] - _em[_qp]) * _phi[_j][_qp];
  Real d_k_d_em = _d_k_d_actual_mean_en[_qp] * d_actual_mean_en_d_em;

  if (jvar == _em_id)
  {
    return -_test[_i][_qp] * d_k_d_em * _threshold_energy * std::exp(_em[_qp] + _target[_qp]);
  }
  else if (jvar == _target_id)
  {
    return -_test[_i][_qp] * d_k_d_em * _threshold_energy * std::exp(_em[_qp] + _target[_qp]) * _phi[_j][_qp];
  }
  else
    return 0.0;
}
