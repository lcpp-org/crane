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

class SuperelasticReactionRate : public Material
{
public:
  SuperelasticReactionRate(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void computeQpProperties();

  MaterialProperty<Real> & _reaction_rate;
  MaterialProperty<Real> & _enthalpy_reaction;
  const MaterialProperty<Real> & _reversible_rate;
  const std::vector<Real> & _coefficients;
  const std::vector<std::string> & _participants;
  const VariableValue & _Tgas;

  Real _power_coefficient;
  std::vector<Real> delta_a;
  Real _equilibrium_constant;

};
