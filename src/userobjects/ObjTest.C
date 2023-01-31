//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ObjTest.h"

registerMooseObject("CraneApp", ObjTest);

InputParameters
ObjTest::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params.addRequiredParam<Real>("reaction_coefficient", "The area of the electrode or plasma.");
  return params;
}

ObjTest::ObjTest(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _reaction_coefficient(getParam<Real>("reaction_coefficient"))
{
}

Real
ObjTest::reaction_coefficient() const
{
  return _reaction_coefficient;
}

void
ObjTest::initialize()
{
}

void
ObjTest::execute()
{
}

void
ObjTest::finalize()
{
}
