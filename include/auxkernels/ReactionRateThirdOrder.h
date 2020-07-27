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

#pragma once

#include "AuxKernel.h"

class ReactionRateThirdOrder;

template <>
InputParameters validParams<ReactionRateThirdOrder>();

class ReactionRateThirdOrder : public AuxKernel
{
public:
  ReactionRateThirdOrder(const InputParameters & parameters);

  virtual ~ReactionRateThirdOrder() {}
  virtual Real computeValue();

protected:
  const VariableValue & _v;
  const VariableValue & _w;
  const VariableValue & _x;
  const MaterialProperty<Real> & _reaction_coeff;
};

