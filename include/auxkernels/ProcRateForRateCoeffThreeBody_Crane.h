//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
