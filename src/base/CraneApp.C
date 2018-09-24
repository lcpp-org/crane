#include "CraneApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

template <>
InputParameters
validParams<CraneApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

CraneApp::CraneApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  CraneApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  CraneApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  CraneApp::registerExecFlags(_factory);
}

CraneApp::~CraneApp() {}

void
CraneApp::registerApps()
{
  registerApp(CraneApp);
}

void
CraneApp::registerObjects(Factory & factory)
{
    Registry::registerObjectsTo(factory, {"CraneApp"});
}

void
CraneApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  Registry::registerActionsTo(action_factory, {"CraneApp"});
  syntax.registerActionSyntax("ChemicalReactions", "ChemicalReactionsSolo");
  // Scalar network actions
  syntax.registerActionSyntax("AddScalarReactions", "ChemicalReactions/ScalarNetwork");
  syntax.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/ScalarNetwork");

  // Spatial network actions
  syntax.registerActionSyntax("AddReactions", "ChemicalReactions/Network");
  syntax.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/Network");

  // Zapdos network actions
  syntax.registerActionSyntax("AddZapdosReactions", "ChemicalReactions/ZapdosNetwork");
  syntax.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/ZapdosNetwork");

  // Deprecated actions - to be removed
  // syntax.registerActionSyntax("ScalarTest", "ChemicalReactions/ScalarTest");
  // syntax.registerActionSyntax("AddReactions", "ChemicalReactions/Network");
  // syntax.registerActionSyntax("ChemicalReactionsBase", "ChemicalReactions/Reactions");
  // syntax.registerActionSyntax("AddScalarBase", "ChemicalReactions/Reactions");
}

void
CraneApp::registerObjectDepends(Factory & /*factory*/)
{
}

void
CraneApp::associateSyntaxDepends(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}

void
CraneApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execution flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
CraneApp__registerApps()
{
  CraneApp::registerApps();
}

extern "C" void
CraneApp__registerObjects(Factory & factory)
{
  CraneApp::registerObjects(factory);
}

extern "C" void
CraneApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  CraneApp::associateSyntax(syntax, action_factory);
}

extern "C" void
CraneApp__registerExecFlags(Factory & factory)
{
  CraneApp::registerExecFlags(factory);
}
