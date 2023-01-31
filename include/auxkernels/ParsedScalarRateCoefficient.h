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

#include "AuxScalarKernel.h"
#include "FunctionParserUtils.h"
// #include "ValueProvider.h"

/**
 * Constant auxiliary value
 */
class ParsedScalarRateCoefficient : public AuxScalarKernel, public FunctionParserUtils<false>
{
public:
  ParsedScalarRateCoefficient(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~ParsedScalarRateCoefficient() {}

protected:
  /**
   * AuxKernels MUST override computeValue.  computeValue() is called on
   * every quadrature point.  For Nodal Auxiliary variables those quadrature
   * points coincide with the nodes.
   */
  virtual Real computeValue() override;

  std::string _function;

  unsigned int _nargs;
  std::vector<const VariableValue *> _args;
  const VariableValue & _reduced_field;
  std::vector<std::string> _constant_names;
  std::vector<std::string> _constant_expressions;
  // const ValueProvider & _data;

  SymFunctionPtr _func_F;
};
