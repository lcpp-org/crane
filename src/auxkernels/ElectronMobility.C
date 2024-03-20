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
#include "MooseUtils.h"

registerMooseObject("CraneApp", ElectronMobility);

InputParameters
ElectronMobility::validParams()
{
 InputParameters params = AuxScalarKernel::validParams();
 params.addRequiredParam<FileName>("file_location",
                                   "The name of the file that stores the mobility table.");
 params.addCoupledVar("reduced_field", "The electron mobility.");
 return params;
}

ElectronMobility::ElectronMobility(const InputParameters & parameters)
 : AuxScalarKernel(parameters),
  _reduced_field(coupledScalarValue("reduced_field"))
{
  std::string file_name = getParam<FileName>("file_location") + "/" + "electron_mobility.txt";
  MooseUtils::checkFileReadable(file_name);
  const char * charPath = file_name.c_str();
  std::ifstream myfile(charPath);
  Real value;

  std::vector<Real> reduced_field;
  std::vector<Real> mobility;
  if (myfile.is_open())
  {
    while (myfile >> value)
    {
      reduced_field.push_back(value);
      myfile >> value;
      mobility.push_back(value);
    }
    myfile.close();
  }
  else
    mooseError("Unable to open file");

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
