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
#include "SplineInterpolation.h"
//#include "LinearInterpolation.h"

class ADZapdosEEDFRateConstant : public ADMaterial
{
public:
  static InputParameters validParams();
  ADZapdosEEDFRateConstant(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  //std::unique_ptr<LinearInterpolation> _coefficient_interpolation;
  SplineInterpolation _coefficient_interpolation;

  Real _r_units;
  ADMaterialProperty<Real> & _rate_coefficient;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;

  using ADMaterial::_communicator;
};
