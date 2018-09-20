/****************************************************************/
/*                      DO NOT MODIFY THIS HEADER               */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*              (c) 2010 Battelle Energy Alliance, LLC          */
/*                      ALL RIGHTS RESERVED                     */
/*                                                              */
/*              Prepared by Battelle Energy Alliance, LLC       */
/*              Under Contract No. DE-AC07-05ID14517            */
/*              With the U. S. Department of Energy             */
/*                                                              */
/*              See COPYRIGHT for full restrictions             */
/****************************************************************/
#ifndef EEDFRATECONSTANTTOWNSEND_H_
#define EEDFRATECONSTANTTOWNSEND_H_

#include "Material.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"

class EEDFRateConstantTownsend;

template <>
InputParameters validParams<EEDFRateConstantTownsend>();

class EEDFRateConstantTownsend : public Material
{
public:
  EEDFRateConstantTownsend(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;

  Real _r_units;
  std::string _coefficient_format;
  MaterialProperty<Real> & _reaction_rate;
  MaterialProperty<Real> & _townsend_coefficient;
  MaterialProperty<Real> & _energy_elastic;
  MaterialProperty<Real> & _d_k_d_en;
  MaterialProperty<Real> & _d_alpha_d_en;
  const MaterialProperty<Real> & _n_gas;
  const MaterialProperty<Real> & _massIncident;
  const MaterialProperty<Real> & _massTarget;

  const VariableValue & _target_species;
  const VariableValue & _em;
  const VariableValue & _mean_en;

  bool _elastic_collision;
};

#endif // EEDFRATECONSTANTTOWNSEND_H_
