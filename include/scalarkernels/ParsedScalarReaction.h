//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ParsedODEKernel.h"
#include "ParsedScalarReaction.h"
#include "SplineInterpolation.h"

class ParsedScalarReaction : public ParsedODEKernel
{
public:
  ParsedScalarReaction(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  SplineInterpolation _temperature_interpolation;

  std::string _sampling_format;
  // int _v_var;
  // const VariableValue & _v;
  //
  // Real _n_gas;
  // Real _stoichiometric_coeff;
  // bool _v_eq_u;

};
