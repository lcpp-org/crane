#include "SuperelasticReactionRate.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", SuperelasticReactionRate);

template <>
InputParameters
validParams<SuperelasticReactionRate>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<std::string>("original_reaction", "The original (reversible) reaction from which this reaction was derived.");
  // params.addRequiredParam<Real>("reaction_rate_value", "The value of the reaction rate (constant).");
  params.addRequiredParam<std::vector<std::string>>("reactants", "The reactants in the reaction.");
  params.addRequiredParam<std::vector<std::string>>("products", "The products in the reaction.");
  params.addRequiredParam<std::vector<Real>>("stoichiometric_coeff", "The coefficients of each reactant and product.");
  params.addRequiredParam<std::vector<std::string>>("all_species", "All reaction participants.");
  // params.addRequiredParam<std::string>("file_location", "The name of the file that stores the reaction rate tables.");
  params.addCoupledVar("gas_temperature", "The temperature of the background gas. Needed for rate constant calculation. Default: 300 K.");
  return params;
}

SuperelasticReactionRate::SuperelasticReactionRate(const InputParameters & parameters)
  : Material(parameters),
    _reaction_rate(declareProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _reversible_rate(getMaterialProperty<Real>("k_" + getParam<std::string>("original_reaction"))),
    // _rate_value(getParam<Real>("reaction_rate_value")),
    _Tgas(isCoupled("gas_temperature") ? coupledValue("gas_temperature") : _zero)
{
}

void
SuperelasticReactionRate::computeQpProperties()
{
  // _reaction_rate[_qp] = _rate_value;
  _reaction_rate[_qp] = 1.0;
  std::cout << _reversible_rate[_qp] << std::endl;
}
