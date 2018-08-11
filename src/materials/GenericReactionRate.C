#include "GenericReactionRate.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", GenericReactionRate);

template <>
InputParameters
validParams<GenericReactionRate>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("reaction_rate_value", "The value of the reaction rate (constant).");

  return params;
}

GenericReactionRate::GenericReactionRate(const InputParameters & parameters)
  : Material(parameters),
    _reaction_rate(declareProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _rate_value(getParam<Real>("reaction_rate_value"))
{}

void
GenericReactionRate::computeQpProperties()
{
  _reaction_rate[_qp] = _rate_value;
}
