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

#ifndef PROCRATEFORRATECOEFF_TOWNSEND_CRANE_H
#define PROCRATEFORRATECOEFF_TOWNSEND_CRANE_H

#include "AuxKernel.h"

class ProcRateForRateCoeff_Townsend_Crane;

template <>
InputParameters validParams<ProcRateForRateCoeff_Townsend_Crane>();

class ProcRateForRateCoeff_Townsend_Crane : public AuxKernel
{
public:
  ProcRateForRateCoeff_Townsend_Crane(const InputParameters & parameters);
  virtual ~ProcRateForRateCoeff_Townsend_Crane() {}
  virtual Real computeValue();
protected:

  Real _r_units;
  std::string _reaction_coeff_name;
  std::string _reaction_name;

  const MaterialProperty<Real> & _diffem;
  const MaterialProperty<Real> & _muem;
  const MaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _d_iz_d_actual_mean_en;
  const MaterialProperty<Real> & _d_muem_d_actual_mean_en;
  const MaterialProperty<Real> & _d_diffem_d_actual_mean_en;

  const VariableValue & _mean_en;
  const VariableGradient & _grad_potential;
  const VariableValue & _em;
  const VariableGradient & _grad_em;
  unsigned int _mean_en_id;
  unsigned int _potential_id;
  unsigned int _em_id;
  const VariableValue & _target;
  unsigned int _target_id;
};

#endif /* PROCRATEFORRATECOEFF_TOWNSEND_CRANE_H */
