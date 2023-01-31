//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "DensityLogConvertScalar.h"

registerMooseObject("CraneApp", DensityLogConvertScalar);

InputParameters
DensityLogConvertScalar::validParams()
{
  InputParameters params = AuxScalarKernel::validParams();
  params.addRequiredCoupledVar("density_log", "The log of a density variable.");
  params.addRequiredParam<bool>("use_moles", "Whether to convert from units of moles to #.");
  return params;
}

DensityLogConvertScalar::DensityLogConvertScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _density_log(coupledScalarValue("density_log")),
    _convert_moles(getParam<bool>("use_moles"))
    // _N_A(getMaterialProperty<Real>("N_A"))
{
}

Real
DensityLogConvertScalar::computeValue()
{
  if (_convert_moles)
    return std::exp(_density_log[_i]) * 6.022e23;

  else
    return std::exp(_density_log[_i]);
}
