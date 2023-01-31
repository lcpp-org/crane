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

#include "GeneralUserObject.h"

class ObjTest : public GeneralUserObject
{
public:
  ObjTest(const InputParameters & parameters);

  static InputParameters validParams();

  Real reaction_coefficient() const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  Real _reaction_coefficient;
};
