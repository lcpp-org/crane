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

#ifndef PROCRATEFORRATECOEFFTHREEBODY_CRANE_H
#define PROCRATEFORRATECOEFFTHREEBODY_CRANE_H

#include "AuxKernel.h"

class ProcRateForRateCoeffThreeBody_Crane;

template <>
InputParameters validParams<ProcRateForRateCoeffThreeBody_Crane>();

class ProcRateForRateCoeffThreeBody_Crane : public AuxKernel
{
public:
  ProcRateForRateCoeffThreeBody_Crane(const InputParameters & parameters);

  virtual ~ProcRateForRateCoeffThreeBody_Crane() {}
  virtual Real computeValue();

protected:


  const VariableValue & _v;
  const VariableValue & _w;
  const VariableValue & _vv;
  const MaterialProperty<Real> & _reaction_coeff;
};

#endif // ProcRateForRateCoeffThreeBody_Crane_H
