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

class TownsendCoefficientLinear : public ADMaterial
{
public:
  static InputParameters validParams();
  TownsendCoefficientLinear(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  //SplineInterpolation _coefficient_interpolation;
  LinearInterpolation _coefficient_interpolation;
  
  std::string _coefficient_format;
  ADMaterialProperty<Real> & _townsend_coefficient;
  const MaterialProperty<Real> & _massIncident;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;

};
