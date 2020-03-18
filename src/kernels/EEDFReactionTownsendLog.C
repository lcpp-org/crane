#include "EEDFReactionTownsendLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", EEDFReactionTownsendLog);

template <>
InputParameters
validParams<EEDFReactionTownsendLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("mean_energy", "The electron mean energy.");
  params.addRequiredCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
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

EEDFReactionTownsendLog::EEDFReactionTownsendLog(const InputParameters & parameters)
  : Kernel(parameters),

    _r_units(1. / getParam<Real>("position_units")),
    _diffem(getMaterialProperty<Real>("diffem")),
    _muem(getMaterialProperty<Real>("muem")),
    _alpha(getMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                     getParam<std::string>("reaction"))),
    _d_iz_d_actual_mean_en(getMaterialProperty<Real>("d_alpha" + getParam<std::string>("number") +
                                                     "_d_en_" + getParam<std::string>("reaction"))),
    _d_muem_d_actual_mean_en(getMaterialProperty<Real>("d_muem_d_actual_mean_en")),
    _d_diffem_d_actual_mean_en(getMaterialProperty<Real>("d_diffem_d_actual_mean_en")),
    _mean_en(coupledValue("mean_energy")),
    _grad_potential(coupledGradient("potential")),
    _em(coupledValue("electrons")),
    _grad_em(coupledGradient("electrons")),
    _mean_en_id(coupled("mean_energy")),
    _potential_id(coupled("potential")),
    _em_id(coupled("electrons")),
    _target(isCoupled("target") ? coupledValue("target") : _zero),
    _target_id(isCoupled("target") ? coupled("target") : 12345678),
    _coefficient(getParam<Real>("coefficient"))
{
}

EEDFReactionTownsendLog::~EEDFReactionTownsendLog() {}

Real
EEDFReactionTownsendLog::computeQpResidual()
{
  Real electron_flux_mag = (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                            _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                               .norm();

  return -_test[_i][_qp] * _alpha[_qp] * std::exp(_target[_qp]) * electron_flux_mag * _coefficient;
}

Real
EEDFReactionTownsendLog::computeQpJacobian()
{
  if (_var.number() == _em_id)
  {
    Real actual_mean_en = std::exp(_mean_en[_qp] - _em[_qp]);

    Real d_iz_d_em = _d_iz_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];
    Real d_muem_d_em = _d_muem_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];
    Real d_diffem_d_em = _d_diffem_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];

    RealVectorValue electron_flux =
        -_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
        _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units;
    RealVectorValue d_electron_flux_d_em =
        -d_muem_d_em * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
        _muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) * _phi[_j][_qp] -
        d_diffem_d_em * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units -
        _diffem[_qp] * std::exp(_em[_qp]) * _phi[_j][_qp] * _grad_em[_qp] * _r_units -
        _diffem[_qp] * std::exp(_em[_qp]) * _grad_phi[_j][_qp] * _r_units;
    Real electron_flux_mag = electron_flux.norm();
    Real d_electron_flux_mag_d_em = electron_flux * d_electron_flux_d_em /
                                    (electron_flux_mag + std::numeric_limits<double>::epsilon());

    Real d_iz_term_d_em = (electron_flux_mag * d_iz_d_em + _alpha[_qp] * d_electron_flux_mag_d_em);

    return -_test[_i][_qp] * d_iz_term_d_em * std::exp(_target[_qp]) * _coefficient;
  }
  else if (_var.number() == _target_id)
  {
    RealVectorValue electron_flux =
        -_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
        _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units;
    Real electron_flux_mag = electron_flux.norm();
    return -_test[_i][_qp] * _alpha[_qp] * std::exp(_target[_qp]) * electron_flux_mag *
           _phi[_j][_qp] * _coefficient;
  }
  else
    return 0;
}

Real
EEDFReactionTownsendLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real actual_mean_en = std::exp(_mean_en[_qp] - _em[_qp]);

  Real d_iz_d_mean_en = _d_iz_d_actual_mean_en[_qp] * actual_mean_en * _phi[_j][_qp];
  Real d_iz_d_em = _d_iz_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];
  Real d_muem_d_mean_en = _d_muem_d_actual_mean_en[_qp] * actual_mean_en * _phi[_j][_qp];
  Real d_muem_d_em = _d_muem_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];
  Real d_diffem_d_mean_en = _d_diffem_d_actual_mean_en[_qp] * actual_mean_en * _phi[_j][_qp];
  Real d_diffem_d_em = _d_diffem_d_actual_mean_en[_qp] * actual_mean_en * -_phi[_j][_qp];

  RealVectorValue electron_flux =
      -_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
      _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units;
  RealVectorValue d_electron_flux_d_potential =
      -_muem[_qp] * -_grad_phi[_j][_qp] * _r_units * std::exp(_em[_qp]);
  RealVectorValue d_electron_flux_d_mean_en =
      -d_muem_d_mean_en * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
      d_diffem_d_mean_en * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units;
  RealVectorValue d_electron_flux_d_em =
      -d_muem_d_em * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
      _muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) * _phi[_j][_qp] -
      d_diffem_d_em * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units -
      _diffem[_qp] * std::exp(_em[_qp]) * _phi[_j][_qp] * _grad_em[_qp] * _r_units -
      _diffem[_qp] * std::exp(_em[_qp]) * _grad_phi[_j][_qp] * _r_units;
  Real electron_flux_mag = electron_flux.norm();
  Real d_electron_flux_mag_d_potential =
      electron_flux * d_electron_flux_d_potential /
      (electron_flux_mag + std::numeric_limits<double>::epsilon());
  Real d_electron_flux_mag_d_mean_en = electron_flux * d_electron_flux_d_mean_en /
                                       (electron_flux_mag + std::numeric_limits<double>::epsilon());
  Real d_electron_flux_mag_d_em = electron_flux * d_electron_flux_d_em /
                                  (electron_flux_mag + std::numeric_limits<double>::epsilon());

  Real d_iz_term_d_potential = (_alpha[_qp] * d_electron_flux_mag_d_potential);
  Real d_iz_term_d_mean_en =
      (electron_flux_mag * d_iz_d_mean_en + _alpha[_qp] * d_electron_flux_mag_d_mean_en);
  Real d_iz_term_d_em = (electron_flux_mag * d_iz_d_em + _alpha[_qp] * d_electron_flux_mag_d_em);

  if (jvar == _potential_id)
    return -_test[_i][_qp] * d_iz_term_d_potential * std::exp(_target[_qp]) * _coefficient;

  else if (jvar == _mean_en_id)
    return -_test[_i][_qp] * d_iz_term_d_mean_en * std::exp(_target[_qp]) * _coefficient;

  else if (jvar == _em_id && _var.number() != _em_id)
    return -_test[_i][_qp] * d_iz_term_d_em * std::exp(_target[_qp]) * _coefficient;
  else if (jvar == _target_id && _var.number() != _target_id)
    return -_test[_i][_qp] * _alpha[_qp] * electron_flux_mag * _phi[_j][_qp] *
           std::exp(_target[_qp]) * _coefficient;

  else
    return 0.0;
}
