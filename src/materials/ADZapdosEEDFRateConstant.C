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
  params.addRequiredParam<std::string>(
      "file_location", "The name of the file that stores the reaction rate tables.");
  params.addParam<bool>("elastic_collision", false, "If the reaction is elastic (true/false).");
  params.addCoupledVar("target_species", "The target species in this collision.");
  params.addCoupledVar("mean_en", "The electron mean energy in log form.");
  params.addCoupledVar("em", "The electron density.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

template <ComputeStage compute_stage>
ADZapdosEEDFRateConstant<compute_stage>::ADZapdosEEDFRateConstant(
    const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _rate_coefficient(declareADProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _em(adCoupledValue("em")),
    _mean_en(isCoupled("mean_en") ? adCoupledValue("mean_en") : _em)
{
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

  _coefficient_interpolation = libmesh_make_unique<LinearInterpolation>(val_x, rate_coefficient);
}

template <ComputeStage compute_stage>
void
ADZapdosEEDFRateConstant<compute_stage>::computeQpProperties()
{
  _rate_coefficient[_qp].value() =
      _coefficient_interpolation->sample(std::exp(_mean_en[_qp].value() - _em[_qp].value()));
  _rate_coefficient[_qp].derivatives() =
      _coefficient_interpolation->sample(std::exp(_mean_en[_qp].value() - _em[_qp].value())) *
      std::exp(_mean_en[_qp].value() - _em[_qp].value()) *
      (_mean_en[_qp].derivatives() - _em[_qp].derivatives());

  if (_rate_coefficient[_qp] < 0.0)
  {
    _rate_coefficient[_qp] = 0.0;
  }
}

template <>
void
ADZapdosEEDFRateConstant<RESIDUAL>::computeQpProperties()
{
  _rate_coefficient[_qp] = _coefficient_interpolation->sample(std::exp(_mean_en[_qp] - _em[_qp]));
  if (_rate_coefficient[_qp] < 0.0)
  {
    _rate_coefficient[_qp] = 0.0;
    // _d_k_d_en[_qp] = 0.0;
  }
}
