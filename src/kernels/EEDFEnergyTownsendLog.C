#include "EEDFEnergyTownsendLog.h"

registerMooseObject("CraneApp", EEDFEnergyTownsendLog);

template <>
InputParameters
validParams<EEDFEnergyTownsendLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addParam<bool>("elastic_collision", false, "If the collision is elastic.");
  params.addRequiredParam<std::string>("reaction", "The reaction that is adding/removing energy.");
  params.addParam<Real>("threshold_energy", 0.0, "Energy required for reaction to take place.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addCoupledVar("target",
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

EEDFEnergyTownsendLog::EEDFEnergyTownsendLog(const InputParameters & parameters)
  : Kernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _elastic(getParam<bool>("elastic_collision")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _diffem(getMaterialProperty<Real>("diffem")),
    _muem(getMaterialProperty<Real>("muem")),
    _alpha(getMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                     getParam<std::string>("reaction"))),
    _d_iz_d_actual_mean_en(getMaterialProperty<Real>("d_alpha" + getParam<std::string>("number") +
                                                     "_d_en_" + getParam<std::string>("reaction"))),
    _d_muem_d_actual_mean_en(getMaterialProperty<Real>("d_muem_d_actual_mean_en")),
    _d_diffem_d_actual_mean_en(getMaterialProperty<Real>("d_diffem_d_actual_mean_en")),
    _grad_potential(isCoupled("potential") ? coupledGradient("potential") : _grad_zero),
    _em(coupledValue("electrons")),
    _grad_em(coupledGradient("electrons")),
    _potential_id(coupled("potential")),
    _em_id(coupled("electrons")),
    _target(coupledValue("target")),
    _target_id(coupled("target"))
{
  if (!_elastic && !isParamValid("threshold_energy"))
    mooseError("EEDFEnergyTownsendLog: Elastic collision set to false, but no threshold "
               "energy for this reaction is provided!");
}

EEDFEnergyTownsendLog::~EEDFEnergyTownsendLog() {}

Real
EEDFEnergyTownsendLog::computeQpResidual()
{
  Real electron_flux_mag = (std::exp(_em[_qp]) * (-_muem[_qp] * -_grad_potential[_qp] * _r_units -
                                                  _diffem[_qp] * _grad_em[_qp] * _r_units))
                               .norm();
  Real iz_term = _alpha[_qp] * std::exp(_target[_qp]) * electron_flux_mag;

  return -_test[_i][_qp] * iz_term * _threshold_energy;
}

Real
EEDFEnergyTownsendLog::computeQpJacobian()
{
  Real d_actual_mean_en_d_mean_en = std::exp(_u[_qp] - _em[_qp]) * _phi[_j][_qp];
  Real d_iz_d_mean_en = _d_iz_d_actual_mean_en[_qp] * d_actual_mean_en_d_mean_en;
  Real d_muem_d_mean_en = _d_muem_d_actual_mean_en[_qp] * d_actual_mean_en_d_mean_en;
  Real d_diffem_d_mean_en = _d_diffem_d_actual_mean_en[_qp] * d_actual_mean_en_d_mean_en;

  RealVectorValue electron_flux =
      std::exp(_em[_qp]) *
      (-_muem[_qp] * -_grad_potential[_qp] * _r_units - _diffem[_qp] * _grad_em[_qp] * _r_units);
  RealVectorValue d_electron_flux_d_mean_en =
      std::exp(_em[_qp]) * (-d_muem_d_mean_en * -_grad_potential[_qp] * _r_units -
                            d_diffem_d_mean_en * _grad_em[_qp] * _r_units);
  Real electron_flux_mag = electron_flux.norm();
  Real d_electron_flux_mag_d_mean_en = electron_flux * d_electron_flux_d_mean_en /
                                       (electron_flux_mag + std::numeric_limits<double>::epsilon());
  Real d_iz_term_d_mean_en =
      (electron_flux_mag * d_iz_d_mean_en + _alpha[_qp] * d_electron_flux_mag_d_mean_en);

  return -_test[_i][_qp] * d_iz_term_d_mean_en * std::exp(_target[_qp]) * _threshold_energy;
}

Real
EEDFEnergyTownsendLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real d_actual_mean_en_d_em = -std::exp(_u[_qp] - _em[_qp]) * _phi[_j][_qp];
  Real d_iz_d_em = _d_iz_d_actual_mean_en[_qp] * d_actual_mean_en_d_em;
  Real d_muem_d_em = _d_muem_d_actual_mean_en[_qp] * d_actual_mean_en_d_em;
  Real d_diffem_d_em = _d_diffem_d_actual_mean_en[_qp] * d_actual_mean_en_d_em;

  RealVectorValue electron_flux =
      std::exp(_em[_qp]) *
      (-_muem[_qp] * -_grad_potential[_qp] * _r_units - _diffem[_qp] * _grad_em[_qp] * _r_units);
  RealVectorValue d_electron_flux_d_potential =
      -_muem[_qp] * -_grad_phi[_j][_qp] * _r_units * std::exp(_em[_qp]);
  RealVectorValue d_electron_flux_d_em =
      std::exp(_em[_qp]) * (-d_muem_d_em * -_grad_potential[_qp] * _r_units -
                            _muem[_qp] * -_grad_potential[_qp] * _r_units * _phi[_j][_qp] -
                            d_diffem_d_em * _grad_em[_qp] * _r_units -
                            _diffem[_qp] * _phi[_j][_qp] * _grad_em[_qp] * _r_units -
                            _diffem[_qp] * _grad_phi[_j][_qp] * _r_units);
  Real electron_flux_mag = electron_flux.norm();
  Real d_electron_flux_mag_d_potential =
      electron_flux * d_electron_flux_d_potential /
      (electron_flux_mag + std::numeric_limits<double>::epsilon());
  Real d_electron_flux_mag_d_em = electron_flux * d_electron_flux_d_em /
                                  (electron_flux_mag + std::numeric_limits<double>::epsilon());

  Real d_iz_term_d_potential = (_alpha[_qp] * d_electron_flux_mag_d_potential);
  Real d_iz_term_d_em = (electron_flux_mag * d_iz_d_em + _alpha[_qp] * d_electron_flux_mag_d_em);

  if (jvar == _potential_id)
    return -_test[_i][_qp] * d_iz_term_d_potential * _threshold_energy * std::exp(_target[_qp]);

  else if (jvar == _em_id)
  {
    return -_test[_i][_qp] * d_iz_term_d_em * _threshold_energy * std::exp(_target[_qp]);
  }
  else if (jvar == _target_id)
  {
    return -_test[_i][_qp] * _alpha[_qp] * std::exp(_target[_qp]) * electron_flux.norm() *
           _phi[_j][_qp] * _threshold_energy;
  }

  else
    return 0.0;
}
