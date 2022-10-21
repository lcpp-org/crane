#pragma once

#include "GeneralUserObject.h"
#include "SplineInterpolation.h"

// class Function;

class ValueProvider : public GeneralUserObject
{
public:
  ValueProvider(const InputParameters & parameters);

  static InputParameters validParams();

  Real electron_temperature(const Real E_N) const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  SplineInterpolation _coefficient_interpolation;
  Real _rate_constant;

  std::string _sampling_format;
  std::string _rate_format;
};
