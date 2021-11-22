#include "ScalarDiffusion.h"

registerMooseObject("CraneApp", ScalarDiffusion);

InputParameters
ScalarDiffusion::validParams()
{
  InputParameters params = ODEKernel::validParams();
  params.addRequiredParam<Real>("rate", "The rate of diffusion.");
  return params;
}

ScalarDiffusion::ScalarDiffusion(const InputParameters & parameters)
  : ODEKernel(parameters), _rate(getParam<Real>("rate"))
{
}

Real
ScalarDiffusion::computeQpResidual()
{
  return _rate * _u[_i];
}

Real
ScalarDiffusion::computeQpJacobian()
{
  return _rate;
}

Real
ScalarDiffusion::computeQpOffDiagJacobian(unsigned int)
{
  return 0;
}
