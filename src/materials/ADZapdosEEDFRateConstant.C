#include "ADZapdosEEDFRateConstant.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADZapdosEEDFRateConstant);

template <ComputeStage compute_stage>
InputParameters
ADZapdosEEDFRateConstant<compute_stage>::validParams()
{
  InputParameters params = ADMaterial<compute_stage>::validParams();
  params.addRequiredParam<FileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("position_units", "The units of position.");
  params.addRequiredParam<std::string>("file_location", "The name of the file that stores the reaction rate tables.");
  params.addRequiredParam<std::string>("sampling_format",
    "The format that the rate constant files are in. Options: reduced_field and electron_energy.");
  params.addParam<bool>("elastic_collision", false, "If the reaction is elastic (true/false).");
  params.addCoupledVar("sampler", "The variable used to sample.");
  params.addCoupledVar("target_species", "The target species in this collision.");
  params.addCoupledVar("mean_en", "The electron mean energy in log form.");
  params.addCoupledVar("em", "The electron density.");
  return params;
}

template <ComputeStage compute_stage>
ADZapdosEEDFRateConstant<compute_stage>::ADZapdosEEDFRateConstant(const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _elastic(getParam<bool>("elastic_collision")),
    _reaction_rate(declareADProperty<Real>("k_"+getParam<std::string>("reaction"))),
    _energy_elastic(declareADProperty<Real>("energy_elastic_"+getParam<std::string>("reaction"))),
    _sampling_format(getParam<std::string>("sampling_format")),
    _massIncident(isCoupled("target_species") ? getMaterialProperty<Real>("mass"+(*getVar("target_species",0)).name()) : getMaterialProperty<Real>("mass"+(*getVar("em",0)).name())),
    _massTarget(getMaterialProperty<Real>("mass"+(*getVar("em",0)).name())),
    // _reduced_field(getMaterialProperty<Real>("reduced_field")),
    _em(adCoupledValue("em")),
    _sampler(isCoupled("sampler") ? adCoupledValue("sampler") : _em),
    _mean_en(isCoupled("mean_en") ? adCoupledValue("mean_en") : _em)
{
  std::vector<Real> val_x;
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
      val_x.push_back(value);
      myfile >> value;
      rate_coefficient.push_back(value);
    }
    myfile.close();
  }
  else
    mooseError("Unable to open file");

  _coefficient_interpolation.setData(val_x, rate_coefficient);
}

template <ComputeStage compute_stage>
void
ADZapdosEEDFRateConstant<compute_stage>::computeQpProperties()
{

  if (isCoupled("sampler"))
  {
    _reaction_rate[_qp] = _coefficient_interpolation.sample(_sampler[_qp]);
  }
  else
  {
    //Real actual_mean_energy = std::exp(_mean_en[_qp] - _em[_qp]);
    //_reaction_rate[_qp] = _coefficient_interpolation.sample(actual_mean_energy);
    _reaction_rate[_qp] = _coefficient_interpolation.sample(std::exp(_mean_en[_qp] - _em[_qp]));
  }
  std::cout << _sampler[_qp] << std::endl;

  if (_reaction_rate[_qp] < 0.0)
  {
    _reaction_rate[_qp] = 0.0;
    // _d_k_d_en[_qp] = 0.0;
  }
  // if (_sampling_format == "electron_energy")
  // {
  //   mooseError("Cannot sample with energy currently. Sample with reduced electric field.");
  // }
  // else if (_sampling_format == "reduced_field")
  // {
  //   _reaction_rate[_qp] = _coefficient_interpolation.sample(_reduced_field[_qp]);
  //   // _reaction_rate[_qp] = _reaction_rate[_qp] * 6.022e23; // convert from [dens]/s to [dens]/mol/s
  //   _d_k_d_en[_qp] = _coefficient_interpolation.sampleDerivative(_reduced_field[_qp]);
  // }

  if (_elastic)
  {
    _energy_elastic[_qp] = -3.0 * (_massIncident[_qp] / _massTarget[_qp]) * 2.0 / 3.0 * std::exp(_mean_en[_qp] - _em[_qp]);
  }
  else
    _energy_elastic[_qp] = 0.0;


}
