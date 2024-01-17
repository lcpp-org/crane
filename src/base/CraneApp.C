//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CraneApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
CraneApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  return params;
}

CraneApp::CraneApp(InputParameters parameters) : MooseApp(parameters)
{
  CraneApp::registerAll(_factory, _action_factory, _syntax);
}

CraneApp::~CraneApp() {}

void
CraneApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<CraneApp>(f, af, s);
  Registry::registerObjectsTo(f, {"CraneApp"});
  Registry::registerActionsTo(af, {"CraneApp"});

  /* register custom execute flags, action syntax, etc. here */
  s.registerActionSyntax("AddSpecies", "ChemicalSpecies");
  s.registerActionSyntax("ChemicalReactions", "ChemicalReactionsSolo");
  // Scalar network actions
  s.registerActionSyntax("AddScalarReactions", "ChemicalReactions/ScalarNetwork");
  s.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/ScalarNetwork");

  // Spatial network actions
  s.registerActionSyntax("AddReactions", "ChemicalReactions/Network");
  s.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/Network");

  // Zapdos network actions
//  s.registerActionSyntax("AddZapdosReactions", "ChemicalReactions/ZapdosNetwork");
//  s.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/ZapdosNetwork");

  s.registerActionSyntax("AddScalarReactions", "GlobalReactions/*");
  s.registerActionSyntax("AddZapdosReactions", "Reactions/*");

  s.registerActionSyntax("AddZapdosReactions", "ChemicalReactions/ZapdosNetwork/*");
}

void
CraneApp::registerApps()
{
  registerApp(CraneApp);
  ModulesApp::registerApps();
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
 extern "C" void
 CraneApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
 {
   CraneApp::registerAll(f, af, s);
 }

extern "C" void
CraneApp__registerApps()
{
  CraneApp::registerApps();
}
