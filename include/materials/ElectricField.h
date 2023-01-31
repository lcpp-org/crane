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
#include "SplineInterpolation.h"

class ElectricField : public Material
{
public:
  ElectricField(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void initQpStatefulProperties() override;
  virtual void computeQpProperties() override;

  SplineInterpolation _mobility;

  MaterialProperty<Real> & _reduced_field;
  const MaterialProperty<Real> & _voltage;
  const MaterialProperty<Real> & _gap_length;
  const MaterialProperty<Real> & _resistance;
  const MaterialProperty<Real> & _gap_area;
  MaterialProperty<Real> & _Vdr;
  const VariableValue & _electron_density;
  const VariableValue & _gas_density;
  const MaterialProperty<Real> & _reduced_field_old;
  bool _use_log;
  const MaterialProperty<Real> & _n_gas;

};
