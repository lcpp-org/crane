#pragma once

#include "GeneralUserObject.h"
#include "SplineInterpolation.h"

// class Function;

class BoltzmannSolverScalar : public GeneralUserObject
{
public:
  BoltzmannSolverScalar(const InputParameters & parameters);
  static InputParameters validParams();

  Real test(const int i) const;
  Real electron_mobility() const;
  Real electron_temperature() const;
  Real coefficient_sample(const int i, const Real sampler) const;
  Real electron_temperature_sample(const Real sampler) const;
  Real electron_mobility_sample(const Real sampler) const;

  virtual void initialize();

  virtual void execute();

  virtual void finalize();

protected:
  std::string _file_name;
  std::size_t _nargs;
  std::vector<const VariableValue *> _args;
  std::vector<std::stringstream> _fractions_string;
  std::string _cross_sections;
  const VariableValue & _reduced_field;
  const VariableValue & _plasma_density;
  const VariableValue & _ionization_fraction;
  std::vector<std::string> _reaction_species;
  std::vector<std::string> _reaction_type;
  std::string _bolsig_run;
  bool _output_table;
  std::string _output_file_name;
  std::string _table_variable;
  std::vector<Real> _x_val;
  std::vector<std::vector<Real>> _rate_coefficient;
  std::vector<int> _reaction_number;
  int _num_reactions;
  unsigned int _n_steps;
  Real _cutoff_time;
  Real _conversion_factor;
  // int _table_size;
  std::vector<int> _reaction_line;
  int _mobility_line;
  int _diffusivity_line;
  int _temperature_line;
  unsigned int _timestep_number;
  std::stringstream _field_string;
  std::vector<Real> _electron_mobility;
  std::vector<Real> _electron_temperature;
  int _table_number;
  Real _tempstore;
  std::vector<SplineInterpolation> _coefficient_interpolation;
  SplineInterpolation _temperature_interpolation;
  SplineInterpolation _mobility_interpolation;
};
