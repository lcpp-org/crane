#pragma once

#include "GeneralUserObject.h"
#include "SplineInterpolation.h"

// class Function;

class PolynomialCoefficients : public GeneralUserObject
{
public:
  PolynomialCoefficients(const InputParameters & parameters);

  static InputParameters validParams();

  Real delta_a(const int i) const;
  Real power_coefficient() const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  const std::vector<Real> & _coefficients;
  const std::vector<std::string> & _participants;

  std::vector<std::vector<Real>> _polynomial_coefficients;
  std::vector<Real> _delta_a;
  Real _power_coefficient;
};
