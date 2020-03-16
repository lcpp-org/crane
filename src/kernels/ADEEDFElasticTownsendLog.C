#include "ADEEDFElasticTownsendLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFElasticTownsendLog);

template <ComputeStage compute_stage>
InputParameters
ADEEDFElasticTownsendLog<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addRequiredCoupledVar("potential", "The potential.");
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

template <ComputeStage compute_stage>
ADEEDFElasticTownsendLog<compute_stage>::ADEEDFElasticTownsendLog(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                       getParam<std::string>("reaction"))),
    _massGas(getMaterialProperty<Real>("mass" + (*getVar("target_species", 0)).name())),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("electron_species")),
    _target(adCoupledValue("target_species")),
    _grad_em(adCoupledGradient("electron_species"))
{
  _massem = 9.11e-31;
}

template <ComputeStage compute_stage>
ADReal
ADEEDFElasticTownsendLog<compute_stage>::computeQpResidual()
{
  ADReal electron_flux_mag = (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                              _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                                 .norm();
  ADReal Eel = -3.0 * _massem / _massGas[_qp] * 2.0 / 3. * std::exp(_u[_qp] - _em[_qp]);
  ADReal el_term = _alpha[_qp] * std::exp(_target[_qp]) * electron_flux_mag * Eel;

  // return -_test[_i][_qp] * _alpha[_qp] * electron_flux_mag * Eel;
  return -_test[_i][_qp] * el_term;
}
