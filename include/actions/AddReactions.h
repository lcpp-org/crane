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

#include "AddVariableAction.h"
#include "Action.h"
#include "ChemicalReactionsBase.h"

// class AddReactions : public AddVariableAction
class AddReactions : public ChemicalReactionsBase
{
public:
  AddReactions(const InputParameters & params);

  static InputParameters validParams();

  virtual void act();

protected:
  virtual void addConstantCoefficient(const unsigned & reaction_num);
  virtual void addFunctionCoefficient(const unsigned & reaction_num);
  virtual void addFunctionReaction(const unsigned & reaction_num,
                                 const unsigned & species_num,
                                 const std::string & kernel_name);
  virtual void addConstantReaction(const unsigned & reaction_num,
                                 const unsigned & species_num,
                                 const std::string & kernel_name);
  virtual std::string getReactionKernelName(const unsigned & num_reactants, const bool & is_aux);
  virtual void addAuxRate(const std::string & aux_kernel_name,
                          const unsigned & reaction_num);

  std::string _coefficient_format;
  std::string _log_append;
  std::vector<std::string> _reactant_names;
};
