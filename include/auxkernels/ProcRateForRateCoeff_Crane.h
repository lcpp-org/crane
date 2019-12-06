//* This file is part of Zapdos, an open-source
//* application for the simulation of plasmas
//* https://github.com/shannon-lab/zapdos
//*
//* Zapdos is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

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
