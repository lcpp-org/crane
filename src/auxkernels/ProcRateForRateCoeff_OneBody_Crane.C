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

#include "ProcRateForRateCoeff_OneBody_Crane.h"

registerMooseObject("CraneApp", ProcRateForRateCoeff_OneBody_Crane);

template <>
InputParameters
validParams<ProcRateForRateCoeff_OneBody_Crane>()
{
  InputParameters params = validParams<AuxKernel>();

  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");

  return params;
}

ProcRateForRateCoeff_OneBody_Crane::ProcRateForRateCoeff_OneBody_Crane(const InputParameters & parameters)
  : AuxKernel(parameters),

  _v(coupledValue("v")),
  _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction")))
{
}

Real
ProcRateForRateCoeff_OneBody_Crane::computeValue()
{

  return 6.02e23 * _reaction_coeff[_qp] * std::exp(_v[_qp]);

}
