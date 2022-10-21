#include "ReactionRateFirstOrder.h"

registerMooseObject("CraneApp", ReactionRateFirstOrder);

InputParameters
ReactionRateFirstOrder::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  params.addClassDescription(
      "Reaction rate for one body collisions. User can pass choice of elastic, excitation, or "
      "ionization reaction rate coefficients. Automatically added if track_rates set to true in "
      "the [Reactions] action.");

  return params;
}

ReactionRateFirstOrder::ReactionRateFirstOrder(const InputParameters & parameters)
  : AuxKernel(parameters),
    _v(coupledValue("v")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction")))
{
}

Real
ReactionRateFirstOrder::computeValue()
{

  return 6.02e23 * _reaction_coeff[_qp] * _v[_qp];
}
