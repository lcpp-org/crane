#include "ADEEDFRxnProd.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFRxnProd);

template <ComputeStage compute_stage>
InputParameters
ADEEDFRxnProd<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addRequiredCoupledVar("mean_en", "The electron mean energy.");
  params.addRequiredCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("em", "The electron density.");
  params.addCoupledVar("target", "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addRequiredParam<std::string>("reaction", "Stores the full reaction equation.");
  params.addRequiredParam<std::string>("reaction_coefficient_name",
                                       "Stores the name of the reaction rate/townsend coefficient, "
                                       "unique to each individual reaction.");
  return params;
}

template <ComputeStage compute_stage>
ADEEDFRxnProd<compute_stage>::ADEEDFRxnProd(const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _reaction_coeff_name(getParam<std::string>("reaction_coefficient_name")),
    _reaction_name(getParam<std::string>("reaction")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>(_reaction_coeff_name)),
    _mean_en(adCoupledValue("mean_en")),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("em")),
    _grad_em(adCoupledGradient("em")),
    _target(adCoupledValue("target"))
    //_target(isCoupled("target") ? adCoupledValue("target") : _em),
{
}

template <ComputeStage compute_stage>
ADReal
ADEEDFRxnProd<compute_stage>::computeQpResidual()
{
  ADReal electron_flux_mag = (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                            _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                               .norm();

  return -_test[_i][_qp] * _alpha[_qp] * electron_flux_mag;
}
