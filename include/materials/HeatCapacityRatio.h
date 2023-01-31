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

// #include "Material.h"
#include "SpeciesSum.h"

class HeatCapacityRatio : public SpeciesSum
{
public:
  HeatCapacityRatio(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();
  MaterialProperty<Real> & _gamma_heat;
  const std::vector<std::string> & _species;
  const MaterialProperty<Real> & _species_sum;
  const VariableValue & _Tgas;
  std::vector<std::vector<Real>> _polynomial_coefficients;
  std::vector<Real> _molar_heat_capacity;

private:
  std::vector<const VariableValue *> _vals;

};
