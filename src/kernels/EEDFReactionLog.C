#include "EEDFReactionLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", EEDFReactionLog);

template <>
InputParameters
validParams<EEDFReactionLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("mean_en", "The electron mean energy.");
  params.addRequiredCoupledVar("em", "The electron density.");
  params.addCoupledVar("target",
                       "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addRequiredParam<std::string>("reaction", "Stores the full reaction equation.");
  params.addRequiredParam<Real>(
      "coefficient", "The stoichiometric coefficient of this variable. (Gain or loss term.)");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

EEDFReactionLog::EEDFReactionLog(const InputParameters & parameters)
  : Kernel(parameters),

    _r_units(1. / getParam<Real>("position_units")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _d_k_d_actual_mean_en(getMaterialProperty<Real>("d_k" + getParam<std::string>("number") +
                                                    "_d_en_" + getParam<std::string>("reaction"))),
    _mean_en(coupledValue("mean_en")),
    _em(coupledValue("em")),
    _mean_en_id(coupled("mean_en")),
    _em_id(coupled("em")),
    _target(coupledValue("target")),
    _target_id(coupled("target")),
    _coefficient(getParam<Real>("coefficient"))
{
}

EEDFReactionLog::~EEDFReactionLog() {}

Real
EEDFReactionLog::computeQpResidual()
{
  return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) * _reaction_coeff[_qp] * _coefficient;
}

Real
EEDFReactionLog::computeQpJacobian()
{
  if (_var.number() == _em_id)
  {
    Real actual_mean_en = std::exp(_mean_en[_qp] - _em[_qp]);
    Real d_k_d_em = _d_k_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];

    return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) * d_k_d_em * _coefficient;
  }
  else if (_var.number() == _target_id)
  {
    return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) * _reaction_coeff[_qp] *
           _coefficient * _phi[_j][_qp];
  }
  else
    return 0;
}

Real
EEDFReactionLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  // Define multiplication factor (determining if product or reactant)

  Real actual_mean_en = std::exp(_mean_en[_qp] - _em[_qp]);

  Real d_k_d_mean_en = _d_k_d_actual_mean_en[_qp] * actual_mean_en * _phi[_j][_qp];
  Real d_k_d_em = _d_k_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];

  if (jvar == _mean_en_id)
    return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) * d_k_d_mean_en * _coefficient;

  else if (jvar == _em_id)
    return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) * _coefficient *
           (_reaction_coeff[_qp] * _phi[_j][_qp] + d_k_d_em);
  else if (jvar == _target_id)
    return -_test[_i][_qp] * std::exp(_em[_qp] + _target[_qp]) * _reaction_coeff[_qp] *
           _coefficient * _phi[_j][_qp];

  else
    return 0.0;

  // if (jvar == _potential_id)
  //   return -_test[_i][_qp] * d_iz_term_d_potential;
  //
  // else if (jvar == _mean_en_id)
  //   return -_test[_i][_qp] * d_iz_term_d_mean_en;
  //
  // else if (jvar == _em_id)
  //   return -_test[_i][_qp] * d_iz_term_d_em;
  //
  // else
  //   return 0.0;
}
