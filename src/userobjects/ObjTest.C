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
