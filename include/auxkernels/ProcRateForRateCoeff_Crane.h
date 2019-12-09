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

#ifndef PROCRATEFORRATECOEFF_CRANE_H
#define PROCRATEFORRATECOEFF_CRANE_H

#include "AuxKernel.h"

class ProcRateForRateCoeff_Crane;

template <>
InputParameters validParams<ProcRateForRateCoeff_Crane>();

class ProcRateForRateCoeff_Crane : public AuxKernel
{
public:
  ProcRateForRateCoeff_Crane(const InputParameters & parameters);

  virtual ~ProcRateForRateCoeff_Crane() {}
  virtual Real computeValue();

protected:


  const VariableValue & _v;
  const VariableValue & _w;
  const MaterialProperty<Real> & _reaction_coeff;
};

#endif // ProcRateForRateCoeff_Crane_H
