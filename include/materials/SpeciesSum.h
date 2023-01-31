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

#include "Material.h"

class SpeciesSum : public Material
{
public:
  SpeciesSum(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  MaterialProperty<Real> & _total_sum;

private:
  std::vector<const VariableValue *> _vals;
  // std::vector<const VariableGradient *> _grad_vals;

};
