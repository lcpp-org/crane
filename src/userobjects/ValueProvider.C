//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ValueProvider.h"
#include "Function.h"
#include "CraneUtils.h"

registerMooseObject("CraneApp", ValueProvider);

InputParameters
ValueProvider::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params += CraneUtils::propertyFileParams();
  params.addParam<std::string>("sampling_format", "reduced_field",
    "The format that the rate constant files are in. Options: reduced_field and electron_energy.");
  return params;
}

ValueProvider::ValueProvider(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _sampling_format(getParam<std::string>("sampling_format"))
{
  const auto [reduced_field, electron_temperature] = CraneUtils::getReactionRates(*this);
  _coefficient_interpolation.setData(reduced_field, electron_temperature);
}

Real
ValueProvider::electron_temperature(const Real E_N) const
{
  return _coefficient_interpolation.sample(E_N) * 11600.0;
  // return 51614.665625302761;
  // return 50000.0;
}

void
ValueProvider::initialize()
{
}

void
ValueProvider::execute()
{
}

void
ValueProvider::finalize()
{
}
