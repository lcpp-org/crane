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

#ifndef PROCRATEFORRATECOEFF_ONEBODY_CRANE_H
#define PROCRATEFORRATECOEFF_ONEBODY_CRANE_H

#include "AuxKernel.h"

class ProcRateForRateCoeff_OneBody_Crane;

template <>
InputParameters validParams<ProcRateForRateCoeff_OneBody_Crane>();

class ProcRateForRateCoeff_OneBody_Crane : public AuxKernel
{
public:
  ProcRateForRateCoeff_OneBody_Crane(const InputParameters & parameters);

  virtual ~ProcRateForRateCoeff_OneBody_Crane() {}
  virtual Real computeValue();

protected:


  const VariableValue & _v;
  const MaterialProperty<Real> & _reaction_coeff;
};

#endif // ProcRateForRateCoeff_OneBody_Crane_H
