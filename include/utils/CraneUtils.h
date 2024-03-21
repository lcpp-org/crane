//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "InputParameters.h"

#include "libmesh/libmesh_common.h"

#include <utility>
#include <vector>

class MooseObject;

namespace CraneUtils
{
using Real = libMesh::Real;

/**
 * The prarameters that describe the base of where to retrieve a property
 * file from.
 */
InputParameters propertyPathParams();
/**
 * The parameters that describe the full path of where to retrieve a property
 * file from, split between the folder base and the file name relative
 * to said base.
 */
InputParameters propertyFileParams();

/**
 * Gets the tabulated reaction rates from a file where the file is defined by the parameters
 * in \p object, specified by the parameters "property_file" and "file_location".
 *
 * The parameters in the object should be defined by propertyFileParams().
 *
 * Performs error checking and passes all errors to \p object.
 */
std::pair<std::vector<Real>, std::vector<Real>> getReactionRates(const MooseObject & object);
/**
 * Gets the coefficients from a file where the file base is defined by the parameters in
 * \p object, specified by the parameter "property_file" and the file name specified by
 * \p property_file.
 *
 * The parameters in the object should be defined by propertyPathParams().
 *
 * Performs error checking and passes all errors to \p object.
 */
std::vector<Real> getCoefficients(const MooseObject & object, const std::string & property_file);

namespace internal
{
/**
 * Internal helper used for getting the file name defined by the parameter "property_file"
 * (the file base) and the file name \p property_file.
 *
 * Performs error checking on whether or not the file exists, and reports errors to \p object.
 */
std::string getReactionRateFileName(const MooseObject & object, const std::string & property_file);
} // namespace internal
} // namespace CraneUtils
