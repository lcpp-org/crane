#include "ADElectronEnergyTermTownsend.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADElectronEnergyTermTownsend);

template <ComputeStage compute_stage>
InputParameters
ADElectronEnergyTermTownsend<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("em", "The electron density.");
  params.addParam<bool>("elastic_collision", false, "If the collision is elastic.");
  params.addRequiredParam<std::string>("reaction", "The reaction that is adding/removing energy.");
  params.addParam<Real>("threshold_energy", 0.0, "Energy required for reaction to take place.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addCoupledVar("target",
                       "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addClassDescription("Adds the change in enthalpy from a chemical reaction to the electron "
                             "and/or gas temperature equations.");
  return params;
}

template <ComputeStage compute_stage>
ADElectronEnergyTermTownsend<compute_stage>::ADElectronEnergyTermTownsend(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha_" + getParam<std::string>("reaction"))),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("em")),
    _grad_em(adCoupledGradient("em")),
    _target(adCoupledValue("target"))
{
}

template <ComputeStage compute_stage>
ADReal
ADElectronEnergyTermTownsend<compute_stage>::computeQpResidual()
{
  _electron_flux_mag = (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                        _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                           .norm();
  // ADReal iz_term = _alpha[_qp] * electron_flux_mag;

  return -_test[_i][_qp] * _alpha[_qp] * _electron_flux_mag * _threshold_energy;
}
