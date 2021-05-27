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

#include "ADMaterial.h"
/* #include "LinearInterpolation.h" */
//#include "SplineInterpolation.h"
#include "LinearInterpolation.h"

class InterpolatedCoefficientLinear : public ADMaterial
{
public:
  static InputParameters validParams();
  InterpolatedCoefficientLinear(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  LinearInterpolation _coefficient_interpolation;

  ADMaterialProperty<Real> & _coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;
};
