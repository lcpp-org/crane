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
#ifndef SUPERELASTICREACTIONRATE_H_
#define SUPERELASTICREACTIONRATE_H_

#include "Material.h"

class SuperelasticReactionRate;

template <>
InputParameters validParams<SuperelasticReactionRate>();

class SuperelasticReactionRate : public Material
{
public:
  SuperelasticReactionRate(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  MaterialProperty<Real> & _reaction_rate;
  Real _rate_value;
  const VariableValue & _Tgas;

};

#endif // SUPERELASTICREACTIONRATE_H_
