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

#include "ADKernel.h"

template <ComputeStage compute_stage>
class ADEEDFReactionTownsendLog : public ADKernel<compute_stage>
{
public:
  static InputParameters validParams();
  ADEEDFReactionTownsendLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();
  usingKernelMembers;

  Real _r_units;

  const ADMaterialProperty(Real) & _diffem;
  const ADMaterialProperty(Real) & _muem;
  const ADMaterialProperty(Real) & _alpha;

  //const ADVariableValue & _mean_en;
  const ADVariableGradient & _grad_potential;
  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _coefficient;
  const ADVariableGradient & _grad_em;
};
