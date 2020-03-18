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
class ADEEDFEnergyLog : public ADKernel<compute_stage>
{
public:
  static InputParameters validParams();
  ADEEDFEnergyLog(const InputParameters & parameters);

protected:
  virtual ADReal computeQpResidual();
  usingKernelMembers;

  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const ADMaterialProperty(Real) & _reaction_coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _target;

  // Threshold energy is just a parameter generally, though elastic collisions require a material property.
  Real _threshold_energy;
};
