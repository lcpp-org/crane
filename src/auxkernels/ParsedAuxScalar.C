//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ParsedAuxScalar.h"

registerMooseObject("CraneApp", ParsedAuxScalar);

InputParameters
ParsedAuxScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params += FunctionParserUtils<false>::validParams();
  params.addClassDescription("Parsed function AuxScalarKernel.");

  params.addRequiredCustomTypeParam<std::string>(
      "function", "FunctionExpression", "function expression");
  params.addCoupledVar("args", "coupled variables");
  params.addParam<std::vector<std::string>>(
      "constant_names", "Vector of constants used in the parsed function (use this for kB etc.)");
  params.addParam<std::vector<std::string>>(
      "constant_expressions",
      "Vector of values for the constants in constant_names (can be an FParser expression)");

  return params;
}

ParsedAuxScalar::ParsedAuxScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    FunctionParserUtils<false>(parameters),
    _function(getParam<std::string>("function")),
    _nargs(coupledScalarComponents("args")),
    _args(_nargs)
{
  // build variables argument
  std::string variables;
  for (unsigned int i = 0; i < _nargs; ++i)
  {
    variables += (i == 0 ? "" : ",") + getScalarVar("args", i)->name();
    _args[i] = &coupledScalarValue("args", i);
  }

  // base function object
  _func_F = SymFunctionPtr(std::make_shared<SymFunction>());

  // set FParser interneal feature flags
  setParserFeatureFlags(_func_F);

  // add the constant expressions
  addFParserConstants(_func_F,
                      getParam<std::vector<std::string>>("constant_names"),
                      getParam<std::vector<std::string>>("constant_expressions"));

  // parse function
  if (_func_F->Parse(_function, variables) >= 0)
    mooseError("Invalid function\n",
               _function,
               "\nin ParsedAuxScalar ",
               name(),
               ".\n",
               _func_F->ErrorMsg());

  // optimize
  if (!_disable_fpoptimizer)
    _func_F->Optimize();

  // just-in-time compile
  if (_enable_jit)
    _func_F->JITCompile();

  // reserve storage for parameter passing bufefr
  _func_params.resize(_nargs);
}

Real
ParsedAuxScalar::computeValue()
{
  for (unsigned int j = 0; j < _nargs; ++j)
    _func_params[j] = (*_args[j])[_i];

  return std::abs(evaluate(_func_F));
}
