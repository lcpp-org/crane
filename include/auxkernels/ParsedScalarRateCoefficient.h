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
