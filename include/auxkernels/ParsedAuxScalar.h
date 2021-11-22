#pragma once

#include "AuxScalarKernel.h"
#include "FunctionParserUtils.h"

/**
 * AuxKernel that evaluates a parsed function expression
 */
class ParsedAuxScalar : public AuxScalarKernel, public FunctionParserUtils<false>
{
public:
  ParsedAuxScalar(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual Real computeValue() override;

  /// function expression
  std::string _function;

  /// coupled variables
  unsigned int _nargs;
  std::vector<const VariableValue *> _args;

  /// function parser object for the resudual and on-diagonal Jacobian
  SymFunctionPtr _func_F;
};
