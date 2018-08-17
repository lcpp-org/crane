#include "GenericEnergyDependentReactionTownsend.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", GenericEnergyDependentReactionTownsend);

template <>
InputParameters
validParams<GenericEnergyDependentReactionTownsend>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<FileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("position_units", "The units of position.");
  params.addRequiredParam<std::string>("file_location", "The name of the file that stores the reaction rate tables.");
  params.addParam<bool>("elastic_collision", false,
                        "Determining whether or not a collision is elastic. Energy change for elastic collisions is calculated on the fly, not pre-assigned.");
  params.addParam<std::string>("reaction_coefficient_format", "townsend",
    "The format of the reaction coefficient. Options: rate or townsend.");
  params.addCoupledVar("target_species", "The heavy (target) species. Optional (default: _n_gas).");
  params.addCoupledVar("mean_en", "The electron mean energy in log form.");
  params.addCoupledVar("em", "The electron density.");

  return params;
}

GenericEnergyDependentReactionTownsend::GenericEnergyDependentReactionTownsend(const InputParameters & parameters)
  : Material(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _coefficient_format(getParam<std::string>("reaction_coefficient_format")),
    _reaction_rate(declareProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _townsend_coefficient(declareProperty<Real>("alpha_"+getParam<std::string>("reaction"))),
    _energy_elastic(declareProperty<Real>("energy_elastic_"+getParam<std::string>("reaction"))),
    _d_k_d_en(declareProperty<Real>("d_k_d_en_"+getParam<std::string>("reaction"))),
    _d_alpha_d_en(declareProperty<Real>("d_alpha_d_en_"+getParam<std::string>("reaction"))),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _massIncident(getMaterialProperty<Real>("massIncident")),
    _massTarget(getMaterialProperty<Real>("massTarget")),

    // Electron information
    _target_species(isCoupled("target_species") ? coupledValue("target_species") : _zero),
    _em(isCoupled("em") ? coupledValue("em") : _zero),
    _mean_en(isCoupled("mean_en") ? coupledValue("mean_en") : _zero),

    // Elastic collision check
    _elastic_collision(getParam<bool>("elastic_collision"))
{
  std::vector<Real> actual_mean_energy;
  std::vector<Real> rate_coefficient;
  std::string file_name = getParam<std::string>("file_location") + "/" + getParam<FileName>("property_file");
  MooseUtils::checkFileReadable(file_name);
  const char * charPath = file_name.c_str();
  std::ifstream myfile(charPath);
  Real value;

  if (myfile.is_open())
  {
    while (myfile >> value)
    {
      actual_mean_energy.push_back(value);
      myfile >> value;
      rate_coefficient.push_back(value);
    }
    myfile.close();
  }
  else
    mooseError("Unable to open file");

  _coefficient_interpolation.setData(actual_mean_energy, rate_coefficient);

  if (_coefficient_format != "rate" && _coefficient_format != "townsend")
    mooseError("Reaction coefficient format '" + _coefficient_format + "' not recognized. Only 'townsend' and 'rate' are accepted.");
}

void
GenericEnergyDependentReactionTownsend::computeQpProperties()
{
  Real actual_mean_energy = std::exp(_mean_en[_qp] - _em[_qp]);
  if (_coefficient_format == "townsend")
  {
    _townsend_coefficient[_qp] = _coefficient_interpolation.sample(actual_mean_energy);
    _d_alpha_d_en[_qp] = _coefficient_interpolation.sampleDerivative(actual_mean_energy);


    if (isCoupled("target_species"))
    {
      _townsend_coefficient[_qp] = _townsend_coefficient[_qp] * std::exp(_target_species[_qp]) / _n_gas[_qp];
    }

    if (_elastic_collision == true)
    {
      _energy_elastic[_qp] = -3.0 * _massIncident[_qp] / _massTarget[_qp] * 2.0 / 3.0 * std::exp(_mean_en[_qp] - _em[_qp]);
    }
  }
  else
  {
    _reaction_rate[_qp] = _coefficient_interpolation.sample(actual_mean_energy);
    _d_k_d_en[_qp] = _coefficient_interpolation.sampleDerivative(actual_mean_energy);
  }
}
