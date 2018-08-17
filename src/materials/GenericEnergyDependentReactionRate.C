#include "GenericEnergyDependentReactionRate.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", GenericEnergyDependentReactionRate);

template <>
InputParameters
validParams<GenericEnergyDependentReactionRate>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<FileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("position_units", "The units of position.");
  params.addRequiredParam<std::string>("file_location", "The name of the file that stores the reaction rate tables.");
  params.addRequiredParam<std::string>("sampling_format",
    "The format that the rate constant files are in. Options: reduced_field and electron_energy.");
  return params;
}

GenericEnergyDependentReactionRate::GenericEnergyDependentReactionRate(const InputParameters & parameters)
  : Material(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _reaction_rate(declareProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _d_k_d_en(declareProperty<Real>("d_k_d_en_"+getParam<std::string>("reaction"))),
    _sampling_format(getParam<std::string>("sampling_format")),
    _reduced_field(getMaterialProperty<Real>("reduced_field"))
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
}

void
GenericEnergyDependentReactionRate::computeQpProperties()
{
  if (_sampling_format == "electron_energy")
  {
    mooseError("Cannot sample with energy currently. Sample with reduced electric field.");
  // _reaction_rate[_qp] = _coefficient_interpolation.sample(actual_mean_energy);
  // _d_k_d_en[_qp] = _coefficient_interpolation.sampleDerivative(actual_mean_energy);
  }
  else if (_sampling_format == "reduced_field")
  {
    _reaction_rate[_qp] = _coefficient_interpolation.sample(_reduced_field[_qp]);
    _d_k_d_en[_qp] = _coefficient_interpolation.sampleDerivative(_reduced_field[_qp]);
  }


}
