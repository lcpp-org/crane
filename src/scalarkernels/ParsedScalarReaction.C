//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ParsedScalarReaction.h"
#include "ParsedODEKernel.h"
#include "CraneUtils.h"

registerMooseObject("CraneApp", ParsedScalarReaction);

InputParameters
ParsedScalarReaction::validParams()
{
  InputParameters params = ParsedODEKernel::validParams();
  params += CraneUtils::propertyFileParams();
  params.addParam<std::string>("sampling_format",
                               "reduced_field",
                               "The format that the rate constant files are in. Options: "
                               "reduced_field and electron_energy.");
  // params.addCoupledVar("v", 0, "Coupled variable 1.");
  // params.addRequiredParam<Real>("n_gas", "The gas density.");
  // params.addRequiredParam<Real>("coefficient", "The stoichiometric coefficient.");
  // params.addParam<bool>("v_eq_u", false, "Whether or not u = v.");
  return params;
}

ParsedScalarReaction::ParsedScalarReaction(const InputParameters & parameters)
  : ParsedODEKernel(parameters), _sampling_format(getParam<std::string>("sampling_format"))
// _v_var(isCoupledScalar("v") ? coupledScalar("v") : -1),
// _v(coupledScalarValue("v")),
// _n_gas(getParam<Real>("n_gas")),
// _stoichiometric_coeff(getParam<Real>("coefficient")),
// _v_eq_u(getParam<bool>("v_eq_u"))
{
  const auto [reduced_field, electron_temperature] = CraneUtils::getReactionRates(*this);
  _temperature_interpolation.setData(reduced_field, electron_temperature);
}

Real
ParsedScalarReaction::computeQpResidual()
{
  // Real mult1;
  // if (isCoupledScalar("v"))
  //   mult1 = _v[_i];
  // else
  //   mult1 = _n_gas;
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i] * mult1;
  return 0;
}

Real
ParsedScalarReaction::computeQpJacobian()
{
  // Real mult1;
  // if (isCoupledScalar("v"))
  //   mult1 = _v[_i];
  // else
  //   mult1 = _n_gas;
  //
  // if (_v_eq_u)
  // {
  //   // return -_stoichiometric_coeff * _data.reaction_coefficient() * 2.0 * _u[_i];
  //   return 0;
  // }
  // else
  // return -_stoichiometric_coeff * _data.reaction_coefficient() * mult1;
  return 0;
}

Real
ParsedScalarReaction::computeQpOffDiagJacobian(unsigned int /*jvar*/)
{
  // Real mult1;
  //
  // if (isCoupledScalar("v"))
  //   mult1 = _v[_i];
  // else
  //   mult1 = _n_gas;
  //
  //
  // if (isCoupledScalar("v"))
  // {
  //   if (jvar == _v_var && !_v_eq_u)
  //   {
  //     // return -_stoichiometric_coeff * _data.reaction_coefficient() * _u[_i];
  //     return 0;
  //   }
  //   else
  //     return 0.0;
  // }
  // else
  return 0.0;
}
