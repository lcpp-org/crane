//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "EEDFRateCoefficientScalar.h"

registerMooseObject("CraneApp", EEDFRateCoefficientScalar);

InputParameters
EEDFRateCoefficientScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addRequiredParam<UserObjectName>("rate_provider", "The name of the UserObject that supplies the rate constant for this timestep.");
  params.addRequiredParam<int>("reaction_number", "The number of the reaction (as defined by the reactions list added to the ChemicalReactions action.)");
  params.addParam<bool>("sample_value", false, "Whether or not to sample outputs. (If output_table = false in BoltzmannSolverScalar, this should be false.)");
  params.addCoupledVar("sample_variable", "The variable being used to sample rate coefficient values. Traditionally either reduced electric field or electron temperature.");
  return params;
}

EEDFRateCoefficientScalar::EEDFRateCoefficientScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
  _data(getUserObject<BoltzmannSolverScalar>("rate_provider")),
  _reaction_number(getParam<int>("reaction_number")),
  _sample_value(getParam<bool>("sample_value")),
  _sampler_var(coupledScalarValue("sample_variable"))
{
}

Real
EEDFRateCoefficientScalar::computeValue()
{
  // std::cout << _data.test(0) << std::endl;
  // return 0.0;
  Real val;
  if (_sample_value)
  {
    val = _data.coefficient_sample(_reaction_number, _sampler_var[0]/1e-21);
    if (val < 0.0)
    {
      return 0.0;
    }
    else
    {
      return val;
    }
    // return _data.coefficient_sample(_reaction_number, _sampler_var[0]/1e-21);
  }
  else
  {
    return _data.test(_reaction_number);
  }
}
