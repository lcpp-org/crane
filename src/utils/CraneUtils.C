//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "CraneUtils.h"

#include "MooseUtils.h"
#include "MooseObject.h"

#include "libmesh/libmesh_common.h"

#include <fstream>

namespace CraneUtils
{
InputParameters
propertyPathParams()
{
  auto params = emptyInputParameters();
  params.addParam<FileName>(
      "file_location",
      ".",
      "The name of the file that stores reaction rate tables (defaults to the current directory).");
  return params;
}

InputParameters
propertyFileParams()
{
  auto params = propertyPathParams();
  params.addRequiredParam<RelativeFileName>(
      "property_file",
      "The file containing interpolation tables for material "
      "properties (within the directory specified by 'file_location').");
  return params;
}

std::pair<std::vector<Real>, std::vector<Real>>
getReactionRates(const MooseObject & object)
{
  const auto & property_file = object.getParam<RelativeFileName>("property_file");
  const auto file_name = internal::getReactionRateFileName(object, property_file);

  std::pair<std::vector<Real>, std::vector<Real>> values;
  auto & [x, y] = values;

  try
  {
    std::ifstream file(file_name.c_str());
    Real value;
    while (file >> value)
    {
      x.push_back(value);
      file >> value;
      y.push_back(value);
    }
    file.close();
  }
  catch (...)
  {
    object.mooseError("Failed to parse the file '", file_name, "'");
  }

  return values;
}

std::vector<Real>
getCoefficients(const MooseObject & object, const std::string & property_file)
{
  const auto file_name = internal::getReactionRateFileName(object, property_file);

  std::vector<Real> values;

  try
  {
    std::ifstream file(file_name.c_str());
    Real value;
    while (file >> value)
    {
      values.push_back(value);
    }
    file.close();
  }
  catch (...)
  {
    object.mooseError("Failed to parse the file '", file_name, "'");
  }

  return values;
}

namespace internal
{
std::string
getReactionRateFileName(const MooseObject & object, const std::string & property_file)
{
  const auto & file_location = object.getParam<FileName>("file_location");
  if (file_location.empty())
    object.paramError(
        "file_location",
        "Parameter is empty; if you wish to use the current working directory, use '.'");

  std::string file_name = file_location + "/" + property_file;
  if (!MooseUtils::checkFileReadable(file_name, false, false))
    object.mooseError("Failed to find the reaction rate file '", file_name, "'");

  return file_name;
}

} // namespace internal
} // namespace CraneUtils
