#include "ADEEDFRateConstantTownsend.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFRateConstantTownsend);

template <ComputeStage compute_stage>
InputParameters
ADEEDFRateConstantTownsend<compute_stage>::validParams()
{
  InputParameters params = ADMaterial<compute_stage>::validParams();
  params.addRequiredParam<FileName>(
      "property_file", "The file containing interpolation tables for material properties.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("position_units", "The units of position.");
  params.addRequiredParam<std::string>(
      "file_location", "The name of the file that stores the reaction rate tables.");
  params.addParam<bool>("elastic_collision",
                        false,
                        "Determining whether or not a collision is elastic. Energy change for "
                        "elastic collisions is calculated on the fly, not pre-assigned.");
  params.addParam<std::string>(
      "reaction_coefficient_format",
      "townsend",
      "The format of the reaction coefficient. Options: rate or townsend.");
  /*
  params.addParam<bool>("is_target_aux",
                        false,
                        "Whether the coupled target species is an aux variable or not. (If it is, "
                        "it does not contribute to jacobian terms.)");
  */
  // params.addCoupledVar("target_species", "The heavy (target) species. Optional (default:
  // _n_gas).");
  params.addRequiredCoupledVar("mean_en", "The electron mean energy in log form.");
  params.addRequiredCoupledVar("em", "The electron density.");
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

template <ComputeStage compute_stage>
ADEEDFRateConstantTownsend<compute_stage>::ADEEDFRateConstantTownsend(
    const InputParameters & parameters)
  : ADMaterial<compute_stage>(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _coefficient_format(getParam<std::string>("reaction_coefficient_format")),
    _townsend_coefficient(declareADProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                                  getParam<std::string>("reaction"))),
    //_energy_elastic(declareADProperty<Real>("energy_elastic_" +
    // getParam<std::string>("reaction"))), _target_coupled(declareProperty<bool>("target_coupled_"
    // + getParam<std::string>("reaction"))), _is_target_aux(getParam<bool>("is_target_aux")),
    _massIncident(getMaterialProperty<Real>("mass" + (*getVar("em", 0)).name())),
    /*
    _massTarget(isCoupled("target_species")
                    ? getMaterialProperty<Real>("mass" + (*getVar("target_species", 0)).name())
                    : getMaterialProperty<Real>("mass" + (*getVar("em", 0)).name())),
                    */

    //_target_species(adCoupledValue("target_species")),
    _em(adCoupledValue("em")),
    _mean_en(isCoupled("mean_en") ? adCoupledValue("mean_en") : _em),
    _elastic_collision(getParam<bool>("elastic_collision"))
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

  //_coefficient_interpolation.setData(val_x, rate_coefficient);
  _coefficient_interpolation = libmesh_make_unique<LinearInterpolation>(val_x, rate_coefficient);
}

template <ComputeStage compute_stage>
void
ADEEDFRateConstantTownsend<compute_stage>::computeQpProperties()
{

  _townsend_coefficient[_qp].value() =
      _coefficient_interpolation->sample(std::exp(_mean_en[_qp].value() - _em[_qp].value()));
  _townsend_coefficient[_qp].derivatives() = _coefficient_interpolation->sampleDerivative(std::exp(
                                                 _mean_en[_qp].value() - _em[_qp].value())) *
                                             std::exp(_mean_en[_qp].value() - _em[_qp].value()) *
                                             (_mean_en[_qp].derivatives() - _em[_qp].derivatives());

  /*
  if (isCoupled("target_species"))
  {
    _townsend_coefficient[_qp] =
        _townsend_coefficient[_qp] * std::exp(_target_species[_qp]) / 40.5250683981307;
  }
  */

  /*
  if (_elastic_collision == true)
  {
    _energy_elastic[_qp] = -3.0 * _massIncident[_qp] / _massTarget[_qp] * 2.0 / 3.0 *
                           std::exp(_mean_en[_qp] - _em[_qp]);
  }
  */
}

template <>
void
ADEEDFRateConstantTownsend<RESIDUAL>::computeQpProperties()
{
  _townsend_coefficient[_qp] =
      _coefficient_interpolation->sample(std::exp(_mean_en[_qp] - _em[_qp]));
}
