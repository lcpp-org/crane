#ifndef ADDREACTIONS_H
#define ADDREACTIONS_H

#include "AddVariableAction.h"
#include "Action.h"
#include "ChemicalReactionsBase.h"

class AddReactions;

template <>
InputParameters validParams<AddReactions>();

// class AddReactions : public AddVariableAction
class AddReactions : public ChemicalReactionsBase
{
public:
  AddReactions(InputParameters params);

  virtual void act();

protected:
  std::string _coefficient_format;

};

#endif // ADDREACTIONS_H
