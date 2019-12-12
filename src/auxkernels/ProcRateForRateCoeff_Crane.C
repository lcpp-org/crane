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

#include "ProcRateForRateCoeff_Crane.h"

registerMooseObject("CraneApp", ProcRateForRateCoeff_Crane);

template <>
InputParameters
validParams<ProcRateForRateCoeff_Crane>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addClassDescription(
    "Reaction rate for two body collisions in units of #/m^3s. User can pass"
    "choice of elastic, excitation, or ionization reaction rate coefficients");

  return params;
}

ProcRateForRateCoeff_Crane::ProcRateForRateCoeff_Crane(const InputParameters & parameters)
  : AuxKernel(parameters),

  _v(coupledValue("v")),
  _w(coupledValue("w")),
  _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction")))
{
}

Real
ProcRateForRateCoeff_Crane::computeValue()
{

  return 6.02e23 * _reaction_coeff[_qp] * std::exp(_v[_qp]) * std::exp(_w[_qp]);

}
