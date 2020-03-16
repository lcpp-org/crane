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

class ReactionRateEEDFTownsendLog;

template <>
InputParameters validParams<ReactionRateEEDFTownsendLog>();

class ReactionRateEEDFTownsendLog : public AuxKernel
{
public:
  ReactionRateEEDFTownsendLog(const InputParameters & parameters);
  virtual ~ReactionRateEEDFTownsendLog() {}
  virtual Real computeValue();

protected:
  Real _r_units;
  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const MaterialProperty<Real> & _diffem;
  const MaterialProperty<Real> & _muem;
  const MaterialProperty<Real> & _alpha;

  const VariableValue & _mean_en;
  const VariableGradient & _grad_potential;
  const VariableValue & _em;
  const VariableGradient & _grad_em;
  const VariableValue & _target;
};
