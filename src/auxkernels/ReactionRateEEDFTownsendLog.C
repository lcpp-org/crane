/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "ReactionRateEEDFTownsendLog.h"

#include "metaphysicl/raw_type.h"

using MetaPhysicL::raw_value;

registerMooseObject("CraneApp", ReactionRateEEDFTownsendLog);
registerMooseObject("CraneApp", ADReactionRateEEDFTownsendLog);

template <bool is_ad>
InputParameters
ReactionRateEEDFTownsendLogTempl<is_ad>::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addRequiredCoupledVar("mean_energy", "The electron mean energy.");
  params.addRequiredCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addCoupledVar("target",
                       "The coupled target. If none, assumed to be background gas from BOLSIG+.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addRequiredParam<std::string>("reaction", "Stores the full reaction equation.");
  params.addRequiredParam<std::string>("reaction_coefficient_name",
                                       "Stores the name of the reaction rate/townsend coefficient, "
                                       "unique to each individual reaction.");
  return params;
}

template <bool is_ad>
ReactionRateEEDFTownsendLogTempl<is_ad>::ReactionRateEEDFTownsendLogTempl(
    const InputParameters & parameters)
  : AuxKernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _reaction_coeff_name(getParam<std::string>("reaction_coefficient_name")),
    _reaction_name(getParam<std::string>("reaction")),
    _diffem(getGenericMaterialProperty<Real, is_ad>("diffem")),
    _muem(getGenericMaterialProperty<Real, is_ad>("muem")),
    _alpha(getGenericMaterialProperty<Real, is_ad>(_reaction_coeff_name)),
    _mean_en(coupledValue("mean_energy")),
    _grad_potential(coupledGradient("potential")),
    _em(coupledValue("electrons")),
    _grad_em(coupledGradient("electrons")),
    _target(isCoupled("target") ? coupledValue("target") : _zero)
{
}

template <bool is_ad>
Real
ReactionRateEEDFTownsendLogTempl<is_ad>::computeValue()
{

  Real electron_flux_mag =
      raw_value((-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
                 _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
                    .norm());
  return raw_value(_alpha[_qp] * std::exp(_target[_qp]) * electron_flux_mag * 6.022e23);
}

template class ReactionRateEEDFTownsendLogTempl<false>;
template class ReactionRateEEDFTownsendLogTempl<true>;
