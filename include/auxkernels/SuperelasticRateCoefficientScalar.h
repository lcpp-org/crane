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

#ifndef SUPERELASTICRATECOEFFICIENTSCALAR_H
#define SUPERELASTICRATECOEFFICIENTSCALAR_H

#include "AuxScalarKernel.h"
#include "SplineInterpolation.h"

class SuperelasticRateCoefficientScalar;

template <>
InputParameters validParams<SuperelasticRateCoefficientScalar>();

class SuperelasticRateCoefficientScalar : public AuxScalarKernel
{
public:
  SuperelasticRateCoefficientScalar(const InputParameters & parameters);

protected:
  virtual Real computeValue();
  SplineInterpolation _coefficient_interpolation;
  const VariableValue & _sampler_var;
  Real _sampler_const;
  std::string _sampling_format;
};

#endif // SUPERELASTICRATECOEFFICIENTSCALAR_H
