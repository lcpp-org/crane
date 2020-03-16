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
class ADEEDFElasticLog : public ADKernel<compute_stage>
{
public:
  static InputParameters validParams();
  ADEEDFElasticLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();
  usingKernelMembers;

  Real _r_units;
  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty(Real) & _reaction_coefficient;
  const MaterialProperty<Real> & _massGas;

  const ADVariableValue & _em;
  const ADVariableValue & _target;
  Real _massem;
};
