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
#pragma once

#include "Material.h"
/* #include "LinearInterpolation.h" */
#include "SplineInterpolation.h"

class EEDFRateConstant;

template <>
InputParameters validParams<EEDFRateConstant>();

class EEDFRateConstant : public Material
{
public:
  EEDFRateConstant(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  SplineInterpolation _coefficient_interpolation;

  MaterialProperty<Real> & _reaction_rate;
  MaterialProperty<Real> & _d_k_d_en;

  const VariableValue & _sampler;
  const VariableValue & _em;
  const VariableValue & _mean_en;
};
