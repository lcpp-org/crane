#include "EEDFRateConstant.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", EEDFRateConstant);

template <>
InputParameters
validParams<EEDFRateConstant>()
{
  InputParameters params = validParams<Material>();
  params.addRequiredParam<FileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<std::string>(
      "file_location", "The name of the file that stores the reaction rate tables.");
  params.addParam<bool>("elastic_collision", false, "If the reaction is elastic (true/false).");
  params.addCoupledVar("sampler", "The variable used to sample.");
  params.addCoupledVar("target_species", "The target species in this collision.");
  params.addCoupledVar("mean_energy", "The electron mean energy.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

EEDFRateConstant::EEDFRateConstant(const InputParameters & parameters)
  : Material(parameters),
    _reaction_rate(declareProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                         getParam<std::string>("reaction"))),
    _d_k_d_en(declareProperty<Real>("d_k_d_en_" + getParam<std::string>("reaction"))),
    _sampler(isCoupled("sampler") ? coupledValue("sampler") : _zero),
    _em(isCoupled("electrons") ? coupledValue("electrons") : _zero),
    _mean_en(isCoupled("mean_energy") ? coupledValue("mean_energy") : _zero)
{
  if (!isCoupled("sampler"))
    mooseError("Sampling variable is not coupled! Please input the variable (aux or nonlinear) "
               "that will be used to sample from data files.");
  std::vector<Real> val_x;
  std::vector<Real> rate_coefficient;
  std::string file_name =
      getParam<std::string>("file_location") + "/" + getParam<FileName>("property_file");
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

void
EEDFRateConstant::computeQpProperties()
{
  _reaction_rate[_qp] = _coefficient_interpolation.sample(_sampler[_qp]);
  _d_k_d_en[_qp] = _coefficient_interpolation.sampleDerivative(_sampler[_qp]);

  if (_reaction_rate[_qp] < 0.0)
  {
    _reaction_rate[_qp] = 0.0;
  }
}
