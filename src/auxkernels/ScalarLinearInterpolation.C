//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ScalarLinearInterpolation.h"

#include "CraneUtils.h"

registerMooseObject("CraneApp", ScalarLinearInterpolation);

InputParameters
ScalarLinearInterpolation::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params += CraneUtils::propertyFileParams();
  params.addCoupledVar("sampler", 0, "The variable with which the data will be sampled.");
  params.addParam<bool>("use_time", false, "Whether or not to sample with time.");
  params.addParam<bool>(
      "use_log", false, "Whether or not to return the natural logarithm of the sampled data.");
  params.addParam<Real>("scale_factor",
                        1.0,
                        "Multiplies the sampled output by a given factor. Convert from m^3 to "
                        "cm^3, for example.(Optional)");
  params.addParam<Real>("const_sampler", 0, "The value with which the data will be sampled.");
  params.addParam<std::string>("sampling_format",
                               "reduced_field",
                               "The format that the rate constant files are in. Options: "
                               "reduced_field and electron_energy.");
  return params;
}

ScalarLinearInterpolation::ScalarLinearInterpolation(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _sampler_var(coupledScalarValue("sampler")),
    _sampler_const(getParam<Real>("const_sampler")),
    _sampling_format(getParam<std::string>("sampling_format")),
    _use_time(getParam<bool>("use_time")),
    _use_log(getParam<bool>("use_log")),
    _scale_factor(getParam<Real>("scale_factor"))
{
  const auto [x_val, y_val] = CraneUtils::getReactionRates(*this);
  _coefficient_interpolation.setData(x_val, y_val);
}

Real
ScalarLinearInterpolation::computeValue()
{
  Real val = 0;

  try
  {
    if (isCoupledScalar("sampler"))
      val = _coefficient_interpolation.sample(_sampler_var[_i]);
    else if (!isCoupledScalar("sampler") && _use_time)
      // val = _coefficient_interpolation_linear.sample(_t);
      val = _coefficient_interpolation.sample(_t);
    else
      val = _coefficient_interpolation.sample(_sampler_const);

    // Ensure positivity
    if (val < 0.0)
    {
      val = 0.0;
    }
    val = val * _scale_factor;
  }
  catch (const std::exception & err)
  {
    MooseException("Linear interpolation out of range! Cutting the timestep...");
  }

  if (_use_log && val > 0)
    return log(val);
  else if (_use_log && val <= 0)
    return 1e-4;
  else
    return val;
}
