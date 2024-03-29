//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "RateCoefficientProvider.h"
#include "CraneUtils.h"
// #include "Function.h"

registerMooseObject("CraneApp", RateCoefficientProvider);

InputParameters
RateCoefficientProvider::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params += CraneUtils::propertyFileParams();
  params.makeParamNotRequired("property_file");
  params.addCoupledVar("reduced_field", 0, "The value of the reduced electric field [V m^2].");
  params.addParam<std::string>("sampling_format",
                               "reduced_field",
                               "The format that the rate constant files are in. Options: "
                               "reduced_field and electron_energy.");
  params.addParam<std::string>("rate_format",
                               "N/A",
                               "The type of rate being implemented. Options: Constant, EEDF, "
                               "Equation. Note that these ARE case-sensitive.");
  params.addParam<Real>(
      "rate_constant", 0.0, "The rate constant, if applicable. (Default 0 otherwise.)");
  // params.addParam<FunctionName>("function", "1", "A function describing the rate coefficient.");
  // params.addParam<Real>("value", 1.0, "Value of the rate coefficient.");
  // params.addParam<Point>(
  // "point", Point(), "A point in space to be given to the function Default: (0, 0, 0)");
  // params.declareControllable("point");
  // params.addCoupledVar("v", "Additional coupled variables.");
  return params;
}

RateCoefficientProvider::RateCoefficientProvider(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _sampling_format(getParam<std::string>("sampling_format")),
    _rate_format(getParam<std::string>("rate_format")),
    _reduced_field_value(coupledScalarValue("reduced_field"))
// _function(getFunction("function")),
// _point(getParam<Point>("point"))
// _reaction_rate(declareProperty<Real>("k_"+getParam<std::string>("reaction"))),
// _d_k_d_en(declareProperty<Real>("d_k_d_en_"+getParam<std::string>("reaction"))),
// _sampling_format(getParam<std::string>("sampling_format"))
{
  if (_rate_format == "EEDF")
  {
    if (!isParamValid("property_file"))
      paramError("rate_format",
                 "The parameter 'property_file' must be specified with rate_format=EEDF");
    const auto [reduced_field, rate_coefficient] = CraneUtils::getReactionRates(*this);
    _coefficient_interpolation.setData(reduced_field, rate_coefficient);
  }
  else if (_rate_format == "Constant")
  {
    _rate_constant = getParam<Real>("rate_constant");
  }
  // else if (_rate_format == "Equation")
  // {
  //   _rate_constant = _function.value(_t, _point);
  // }
  else if (_rate_format != "Equation")
  {
    mooseError("RateCoefficientProvider: Rate format " + _rate_format + " is not applicable.");
  }
}

Real
RateCoefficientProvider::reaction_coefficient() const
{
  Real reaction_rate = 0;
  if (_rate_format == "EEDF")
  {
    if (_sampling_format == "electron_energy")
    {
      mooseError("Cannot sample with energy currently. Sample with reduced electric field.");
    }
    else if (_sampling_format == "reduced_field")
    {
      reaction_rate = _coefficient_interpolation.sample(_reduced_field_value[0]);

      // reaction_rate = _coefficient_interpolation.sample(7.76697e-20);
      // reaction_rate = _coefficient_interpolation.sample(5.0e-20);

      reaction_rate = reaction_rate * 1e6;

      // _reaction_rate[_qp] = _reaction_rate[_qp] * 6.022e23; // convert from [dens]/s to
      // [dens]/mol/s _d_k_d_en[_qp] =
      // _coefficient_interpolation.sampleDerivative(_reduced_field[_qp]);
    }
  }
  else if (_rate_format == "Constant")
  {
    reaction_rate = _rate_constant;
  }
  else if (_rate_format == "Equation")
  {
    // this functionality is deprecated
    // Does not need to be used
    // reaction_rate = _function.value(_t, _point);
    reaction_rate = 0; // This reaction rate is now calculated as an AuxVariable
  }

  return reaction_rate;
}

Real
RateCoefficientProvider::reaction_coefficient_derivative() const
{
  Real d_k_d_en = 0;

  if (_rate_format == "EEDF")
  {
    if (_sampling_format == "electron_energy")
      mooseError("RateCoefficientProvider: Cannot sample with energy currently.");
    else
    {
      d_k_d_en = _coefficient_interpolation.sampleDerivative(7.76697e-20);
    }
  }
  else if (_rate_format == "Constant")
  {
    d_k_d_en = 0.0;
  }
  else if (_rate_format == "Equation")
  {
    d_k_d_en = 0.0;
  }
  return d_k_d_en;
}

Real
RateCoefficientProvider::electron_temperature(const Real E_N) const
{
  Real Te;

  Te = _coefficient_interpolation.sampleDerivative(E_N);
  return Te;
}

// Real
// RateCoefficientProvider::reduced_field(const Real reduced_field_old, const Real gas_density)
// const
// {
//   Real reduced_field;
//   // reduced_field = _voltage / ( _gap_length + _resistance * current /
//   //   ( _reduced_field_old*mult1 ) ) / mult1;
//   reduced_field = 1000.0 / (0.4 + (1e5*1.0 / (reduced_field_old*gas_density ))) / gas_density;
//
//   return reduced_field;
// }

void
RateCoefficientProvider::initialize()
{
}

void
RateCoefficientProvider::execute()
{
}

void
RateCoefficientProvider::finalize()
{
}
