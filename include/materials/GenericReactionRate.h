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
#ifndef GENERICREACTIONRATE_H_
#define GENERICREACTIONRATE_H_

#include "Material.h"

class GenericReactionRate;

template <>
InputParameters validParams<GenericReactionRate>();

class GenericReactionRate : public Material
{
public:
  GenericReactionRate(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  MaterialProperty<Real> & _reaction_rate;

  Real _rate_value;

};

#endif // GENERICREACTIONRATE_H_
