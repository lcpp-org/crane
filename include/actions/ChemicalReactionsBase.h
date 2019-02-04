#ifndef CHEMICALREACTIONSBASE_H
#define CHEMICALREACTIONSBASE_H

#include "AddVariableAction.h"
#include "Action.h"

class ChemicalReactionsBase;

template <>
InputParameters validParams<ChemicalReactionsBase>();

// class ChemicalReactionsBase : public AddVariableAction
class ChemicalReactionsBase : public Action
{
public:
  ChemicalReactionsBase(InputParameters params);

  virtual void act();

protected:
  const std::vector<NonlinearVariableName> _species;
  const std::vector<NonlinearVariableName> _electron_energy;
  const std::vector<NonlinearVariableName> _gas_energy;
  std::string _input_reactions;
  Real _r_units;
  std::string _sampling_variable;
  bool _use_log;
  // bool _use_moles;

  std::vector<bool> _energy_change;
  // std::vector<VariableName> _potential;
  std::vector<std::vector<Real>> _species_count;
  std::vector<unsigned int> _electron_index;
  std::vector<bool> _reversible_reaction;
  std::vector<bool> _superelastic_reaction;
  // std::vector<std::vector<bool>> _species_electron;
  std::vector<bool> _electron_reaction;
  std::vector<bool> _elastic_collision;
  /// Equilibrium constants for each reaction
  std::vector<Real> _eq_const;
  /// Vector of parsed reactions
  std::vector<Real> _threshold_energy;
  std::vector<std::string> _reaction;
  std::vector<std::string> _reaction_coefficient_name;
  std::vector<bool> _rate_equation;
  std::vector<std::string> _rate_type;
  std::vector<AuxVariableName> _aux_var_name;
  // std::vector<std::string> _rate_coefficient_string;
  std::vector<Real> _rate_coefficient;
  std::vector<Real> _test;
  std::vector<std::vector<std::string>> _reactants;
  std::vector<std::vector<std::string>> _products;
  std::vector<std::string> _all_participants;
  std::vector<std::vector<std::string>> _reaction_participants;
  std::vector<std::vector<Real>> _reaction_stoichiometric_coeff;
  std::vector<int> _superelastic_index;
  std::vector<int> _species_index;
  std::vector<std::vector<Real>> _stoichiometric_coeff;
  std::vector<int> _num_reactants;
  std::vector<int> _num_products;
  std::vector<std::string> _rate_equation_string;
  std::vector<std::string> _reaction_identifier;
  std::vector<std::string> _eedf_reaction_number;
  std::vector<std::string> _reaction_species;
  int _bolsig_reaction;
  /// Number of reactions
  unsigned int _num_reactions;
  std::vector<bool> _electron_energy_term;
  std::vector<NonlinearVariableName> _energy_variable;
};

#endif // CHEMICALREACTIONSBASE_H
