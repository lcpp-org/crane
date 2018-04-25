//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "CraneTestApp.h"
#include "CraneApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<CraneTestApp>()
{
  InputParameters params = validParams<CraneApp>();
  return params;
}

CraneTestApp::CraneTestApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  CraneApp::registerObjectDepends(_factory);
  CraneApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  CraneApp::associateSyntaxDepends(_syntax, _action_factory);
  CraneApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  CraneApp::registerExecFlags(_factory);

  bool use_test_objs = getParam<bool>("allow_test_objects");
  if (use_test_objs)
  {
    CraneTestApp::registerObjects(_factory);
    CraneTestApp::associateSyntax(_syntax, _action_factory);
    CraneTestApp::registerExecFlags(_factory);
  }
}

CraneTestApp::~CraneTestApp() {}

void
CraneTestApp::registerApps()
{
  registerApp(CraneApp);
  registerApp(CraneTestApp);
}

void
CraneTestApp::registerObjects(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new test objects here! */
}

void
CraneTestApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  /* Uncomment Syntax and ActionFactory parameters and register your new test objects here! */
}

void
CraneTestApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execute flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
CraneTestApp__registerApps()
{
  CraneTestApp::registerApps();
}

// External entry point for dynamic object registration
extern "C" void
CraneTestApp__registerObjects(Factory & factory)
{
  CraneTestApp::registerObjects(factory);
}

// External entry point for dynamic syntax association
extern "C" void
CraneTestApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  CraneTestApp::associateSyntax(syntax, action_factory);
}

// External entry point for dynamic execute flag loading
extern "C" void
CraneTestApp__registerExecFlags(Factory & factory)
{
  CraneTestApp::registerExecFlags(factory);
}
