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

#include "ReactionRateThirdOrderLog.h"

registerMooseObject("CraneApp", ReactionRateThirdOrderLog);

template <>
InputParameters
validParams<ReactionRateThirdOrderLog>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addCoupledVar("v", "The first reactant.");
  params.addCoupledVar("w", "The second reactant.");
  params.addCoupledVar("x", "The third reactant.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  params.addClassDescription(
      "Reaction rate for three body collisions. User can pass choice of elastic, excitation, or "
      "ionization reaction rate coefficients. Automatically added if track_rates set to true in "
      "the [Reactions] action.");
  return params;
}

ReactionRateThirdOrderLog::ReactionRateThirdOrderLog(const InputParameters & parameters)
  : AuxKernel(parameters),

    _v(coupledValue("v")),
    _w(coupledValue("w")),
    _x(coupledValue("x")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction")))
{
}

Real
ReactionRateThirdOrderLog::computeValue()
{

  return 6.02e23 * _reaction_coeff[_qp] * std::exp(_v[_qp] + _w[_qp] + _x[_qp]);
}
