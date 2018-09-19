/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "DataReadScalar.h"

registerMooseObject("CraneApp", DataReadScalar);

template <>
InputParameters
validParams<DataReadScalar>()
{
  InputParameters params = validParams<AuxScalarKernel>();
  params.addCoupledVar("sampler", 0, "The variable with which the data will be sampled.");
  params.addParam<Real>("const_sampler", 0, "The value with which the data will be sampled.");
  params.addParam<FileName>("property_file", "",
      "The file containing interpolation tables for material properties.");
  params.addParam<std::string>("file_location", "", "The name of the file that stores the reaction rate tables.");
  params.addParam<std::string>("sampling_format", "reduced_field",
    "The format that the rate constant files are in. Options: reduced_field and electron_energy.");
  return params;
}

DataReadScalar::DataReadScalar(const InputParameters & parameters)
  : AuxScalarKernel(parameters),
    _sampler_var(coupledScalarValue("sampler")),
    _sampler_const(getParam<Real>("const_sampler")),
    _sampling_format(getParam<std::string>("sampling_format"))
{
  std::vector<Real> reduced_field;
  std::vector<Real> sampled_value;
  std::string file_name = getParam<std::string>("file_location") + "/" + getParam<FileName>("property_file");
  MooseUtils::checkFileReadable(file_name);
  const char * charPath = file_name.c_str();
  std::ifstream myfile(charPath);
  Real value;

  if (myfile.is_open())
  {
    while (myfile >> value)
    {
      reduced_field.push_back(value);
      myfile >> value;
      sampled_value.push_back(value);
    }
    myfile.close();
  }
  else
    mooseError("Unable to open file");

  _coefficient_interpolation.setData(reduced_field, sampled_value);
}

Real
DataReadScalar::computeValue()
{
  Real val;
  if (isCoupledScalar("sampler"))
    val = _coefficient_interpolation.sample(_sampler_var[0]);
  else
    val = _coefficient_interpolation.sample(_sampler_const);

  // Ensure positivity
  if (val < 0.0)
  {
    val = 0.0;
    // val = abs(val);
  }
  // if (isCoupledScalar("sampler"))
  // {
  //
  //   return _coefficient_interpolation.sample(_sampler_var[0]);
  // }
  // else
  //   return _coefficient_interpolation.sample(_sampler_const);

  return val;
}
