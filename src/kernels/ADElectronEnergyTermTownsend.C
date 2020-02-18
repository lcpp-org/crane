#include "ADElectronEnergyTermElasticTownsend.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADElectronEnergyTermElasticTownsend);

template <ComputeStage compute_stage>
InputParameters
ADElectronEnergyTermElasticTownsend<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addRequiredCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electron_species", "The electron density.");
  params.addRequiredCoupledVar("target_species", "The target species variable.");
  params.addRequiredParam<std::string>("reaction",
                                       "Stores the name of the reaction (townsend) coefficient, "
                                       "unique to each individual reaction.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  return params;
}

template <ComputeStage compute_stage>
ADElectronEnergyTermElasticTownsend<compute_stage>::ADElectronEnergyTermElasticTownsend(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha_" + getParam<std::string>("reaction"))),
    _massGas(getMaterialProperty<Real>("mass" + (*getVar("target_species", 0)).name())),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("electron_species")),
    _grad_em(adCoupledGradient("electron_species"))
{
  _massem = 9.11e-31;
}

template <ComputeStage compute_stage>
ADReal
ADElectronEnergyTermElasticTownsend<compute_stage>::computeQpResidual()
{
  ADReal electron_flux_mag = (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                              _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                                 .norm();
  ADReal Eel = -3.0 * _massem / _massGas[_qp] * 2.0 / 3. * std::exp(_u[_qp] - _em[_qp]);
  ADReal el_term = _alpha[_qp] * electron_flux_mag * Eel;

  return -_test[_i][_qp] * _alpha[_qp] * electron_flux_mag;
}
