#include "ADEEDFRateConstantTownsend.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFRateConstantTownsend);

InputParameters
ADEEDFRateConstantTownsend::validParams()
{
  InputParameters params = ADMaterial::validParams();
  params.addRequiredParam<FileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<std::string>(
      "file_location", "The name of the file that stores the reaction rate tables.");
  params.addParam<std::string>(
      "reaction_coefficient_format",
      "townsend",
      "The format of the reaction coefficient. Options: rate or townsend.");
  params.addRequiredCoupledVar("mean_energy", "The electron mean energy in log form.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  params.addClassDescription(
      "Adds townsend coefficient for a reaction as a material property. Note that this material is "
      "only intended to be used with the plasma transport software, Zapdos. For any other MOOSE "
      "application (or when using Crane by itself) it is recommended to use rate coefficient "
      "formulation rather than the townsend coefficients, which may be done with the "
      "(AD)EEDFRateConstant material.");
  return params;
}

ADEEDFRateConstantTownsend::ADEEDFRateConstantTownsend(const InputParameters & parameters)
  : ADMaterial(parameters),
    _coefficient_format(getParam<std::string>("reaction_coefficient_format")),
    _townsend_coefficient(declareADProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                                  getParam<std::string>("reaction"))),
    _massIncident(getMaterialProperty<Real>("mass" + (*getVar("electrons", 0)).name())),
    _em(adCoupledValue("electrons")),
    _mean_en(isCoupled("mean_energy") ? adCoupledValue("mean_energy") : _em)
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

  _coefficient_interpolation.setData(val_x, rate_coefficient);
}

void
ADEEDFRateConstantTownsend::computeQpProperties()
{

  _townsend_coefficient[_qp].value() =
      _coefficient_interpolation.sample(std::exp(_mean_en[_qp].value() - _em[_qp].value()));
  _townsend_coefficient[_qp].derivatives() = _coefficient_interpolation.sampleDerivative(std::exp(
                                                 _mean_en[_qp].value() - _em[_qp].value())) *
                                             std::exp(_mean_en[_qp].value() - _em[_qp].value()) *
                                             (_mean_en[_qp].derivatives() - _em[_qp].derivatives());

  if (_townsend_coefficient[_qp].value() < 0)
  {
    _townsend_coefficient[_qp].value() = 0;
    _townsend_coefficient[_qp].derivatives() = 0;
  }
}
