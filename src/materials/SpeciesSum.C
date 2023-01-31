//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SpeciesSum.h"
#include "MooseUtils.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", SpeciesSum);

InputParameters
SpeciesSum::validParams()
{
  InputParameters params = Material::validParams();
  params.addCoupledVar("coupled_vars", "The coupled variables to sum.");
  return params;
}

SpeciesSum::SpeciesSum(const InputParameters & parameters)
  : Material(parameters),
  _total_sum(declareProperty<Real>("species_sum"))
{
  int n = coupledComponents("coupled_vars");

  _vals.resize(n);

  for (unsigned int i=0; i<_vals.size(); ++i)
  {
    _vals[i] = &coupledValue("coupled_vars", i);
  }
}

void
SpeciesSum::computeQpProperties()
{
  _total_sum[_qp] = 0.0;
  for (unsigned int i=0; i<_vals.size(); ++i)
  {
    _total_sum[_qp] += (*_vals[i])[_qp];
  }
}
