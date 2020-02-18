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

template <ComputeStage compute_stage>
class ADEEDFRateConstantTownsend : public ADMaterial<compute_stage>
{
public:
  static InputParameters validParams();
  ADEEDFRateConstantTownsend(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  //SplineInterpolation _coefficient_interpolation;
  ADLinearInterpolation _coefficient_interpolation;
  
  Real _r_units;
  std::string _coefficient_format;
  ADMaterialProperty(Real) & _townsend_coefficient;
  ADMaterialProperty(Real) & _energy_elastic;
  MaterialProperty<bool> & _target_coupled;
  bool _is_target_aux;
  const MaterialProperty<Real> & _massIncident;
  const MaterialProperty<Real> & _massTarget;

  const ADVariableValue & _target_species;
  const ADVariableValue & _em;
  const ADVariableValue & _mean_en;

  bool _elastic_collision;
  
  usingMaterialMembers;
  using ADMaterial<compute_stage>::_communicator;
};
