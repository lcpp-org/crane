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

#include "ProductionThirdOrder.h"

registerMooseObject("CraneApp", ProductionThirdOrder);

template <>
InputParameters
validParams<ProductionThirdOrder>()
{
  InputParameters params = validParams<AuxScalarKernel>();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredCoupledVar("z", "The second variable that is reacting to create u.");
  params.addCoupledVar("rate_coefficient", 0, "Coupled reaction coefficient (if equation-based).");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ProductionThirdOrder::ProductionThirdOrder(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _v(coupledScalarValue("v")),
    _w(coupledScalarValue("w")),
    _z(coupledScalarValue("z")),
    _rate_coefficient(coupledScalarValue("rate_coefficient")),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ProductionThirdOrder::computeValue()
{
  return _stoichiometric_coeff * _rate_coefficient[_i] * _v[_i] * _w[_i] * _z[_i];
}
