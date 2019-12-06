//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
