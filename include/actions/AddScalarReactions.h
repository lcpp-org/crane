#ifndef ADDSCALARREACTIONS_H
#define ADDSCALARREACTIONS_H

#include "AddVariableAction.h"
#include "Action.h"
#include "ChemicalReactionsBase.h"

class AddScalarReactions;

template <>
InputParameters validParams<AddScalarReactions>();

// class ChemicalReactions : public AddVariableAction
class AddScalarReactions : public ChemicalReactionsBase
{
public:
  AddScalarReactions(InputParameters params);
  // AddScalarReactions(InputParameters params) : ChemicalReactionsBase(params) {};

  virtual void act();

protected:

  // std::vector<std::vector<std::string>> _reaction_participants;
  // std::vector<std::vector<Real>> _reaction_stoichiometric_coeff;
//   std::vector<int> _superelastic_index;
//   std::vector<int> _species_index;
//   std::vector<std::vector<Real>> _stoichiometric_coeff;
//   std::vector<int> _num_reactants;
//   std::vector<int> _num_products;
//   std::vector<std::string> _rate_equation_string;
//   /// Number of reactions
//   unsigned int _num_reactions;

};

#endif // ADDSCALARREACTIONS_H
