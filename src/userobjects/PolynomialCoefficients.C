//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PolynomialCoefficients.h"
#include "Function.h"
#include "CraneUtils.h"

registerMooseObject("CraneApp", PolynomialCoefficients);

InputParameters
PolynomialCoefficients::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  params += CraneUtils::propertyPathParams();
  params.addRequiredParam<std::vector<Real>>("stoichiometric_coeff", "The coefficients of each reactant and product.");
  params.addRequiredParam<std::vector<std::string>>("participants", "All reaction participants.");
  return params;
}

PolynomialCoefficients::PolynomialCoefficients(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _coefficients(getParam<std::vector<Real>>("stoichiometric_coeff")),
  _participants(getParam<std::vector<std::string>>("participants"))
{
}

Real
PolynomialCoefficients::delta_a(const int i) const
{
  return _delta_a[i];
}

Real
PolynomialCoefficients::power_coefficient() const
{
  return _power_coefficient;
}

void
PolynomialCoefficients::initialize()
{
  // This material follows the method outlined in:
  // Mikhail S Finko et al 2017 J. Phys. D: Appl. Phys. 50 485201
  // Section 2.2, Equation 13

  // Read the participant species' coefficients from files
  _polynomial_coefficients.resize(_participants.size());
  _power_coefficient = 0.0;
  for (unsigned int i = 0; i < _participants.size(); ++i)
  {
    _power_coefficient += _coefficients[i];  // Finko, equation 7
    _polynomial_coefficients[i] = CraneUtils::getCoefficients(*this, _participants[i] + ".txt");
  }
}

void
PolynomialCoefficients::execute()
{
  // Sum the coefficients
  _delta_a.resize(_polynomial_coefficients[0].size(), 0.0);
  for (unsigned int i = 0; i < _polynomial_coefficients.size(); ++i)
  {
    for (unsigned int j = 0; j < _polynomial_coefficients[i].size(); ++j)
    {
      _delta_a[j] += _coefficients[i] * _polynomial_coefficients[i][j];
    }
  }
}

void
PolynomialCoefficients::finalize()
{
}
