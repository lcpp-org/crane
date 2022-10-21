#include "VariableSum.h"

registerMooseObject("CraneApp", VariableSum);

InputParameters
VariableSum::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addCoupledVar("args", "The variables to sum.");
  return params;
}

VariableSum::VariableSum(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _nargs(coupledScalarComponents("args")),
    _args(_nargs)
{
}

Real
VariableSum::computeValue()
{
  Real variable_sum = 0;
  for (unsigned int i = 0; i < _nargs; ++i)
  {
    // variables += (i == 0 ? "" : ",") + getScalarVar("args", i)->name();
    // _args[i] = &coupledScalarValue("args", i);
    // std::cout << (*_args[i])[_i] << std::endl;
    variable_sum += coupledScalarValue("args", i)[_i];
  }
  return variable_sum;
}
