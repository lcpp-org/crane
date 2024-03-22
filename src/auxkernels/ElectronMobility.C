//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ElectronMobility.h"

#include "CraneUtils.h"

registerMooseObject("CraneApp", ElectronMobility);

InputParameters
ElectronMobility::validParams()
{
 InputParameters params = AuxScalarKernel::validParams();
 params += CraneUtils::propertyFileParams();
 params.makeParamNotRequired("property_file");
 params.set<RelativeFileName>("property_file") = "electron_mobility.txt";
 params.addCoupledVar("reduced_field", "The electron mobility.");
 return params;
}

ElectronMobility::ElectronMobility(const InputParameters & parameters)
 : AuxScalarKernel(parameters),
  _reduced_field(coupledScalarValue("reduced_field"))
{
  const auto [reduced_field, mobility] = CraneUtils::getReactionRates(*this);
  _mobility.setData(reduced_field, mobility);
}

ElectronMobility::~ElectronMobility() {}

Real
ElectronMobility::computeValue()
{
 // return _u_old[_i] * (1 - (_lambda * _dt));
 // return 1.0;
 return _mobility.sample(_reduced_field[_i]);
}
