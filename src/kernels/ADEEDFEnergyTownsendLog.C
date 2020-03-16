#include "ADEEDFEnergyTownsendLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFEnergyTownsendLog);

template <ComputeStage compute_stage>
InputParameters
ADEEDFEnergyTownsendLog<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electron_species", "The electron density.");
  params.addParam<bool>("elastic_collision", false, "If the collision is elastic.");
  params.addRequiredParam<std::string>("reaction", "The reaction that is adding/removing energy.");
  params.addParam<Real>("threshold_energy", 0.0, "Energy required for reaction to take place.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addCoupledVar("target_species",
                       "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addClassDescription("Adds the change in enthalpy from a chemical reaction to the electron "
                             "and/or gas temperature equations.");
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
ADEEDFEnergyTownsendLog<compute_stage>::ADEEDFEnergyTownsendLog(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                       getParam<std::string>("reaction"))),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("electron_species")),
    _grad_em(adCoupledGradient("electron_species")),
    _target(adCoupledValue("target_species"))
{
}

template <ComputeStage compute_stage>
ADReal
ADEEDFEnergyTownsendLog<compute_stage>::computeQpResidual()
{
  _electron_flux_mag = (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                        _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                           .norm();
  // ADReal iz_term = _alpha[_qp] * electron_flux_mag;

  return -_test[_i][_qp] * _alpha[_qp] * std::exp(_target[_qp]) * _electron_flux_mag *
         _threshold_energy;
}
