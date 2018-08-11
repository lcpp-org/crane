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

#ifndef REACTANT2BODY_H
#define REACTANT2BODY_H

#include "NodalKernel.h"

// Forward Declaration
class Reactant2Body;

template <>
InputParameters validParams<Reactant2Body>();

class Reactant2Body : public NodalKernel
{
public:
  Reactant2Body(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  MooseVariable & _coupled_var_A;
  const VariableValue & _v;

  // The reaction coefficient
  Real _rate_coefficient;
  Real _stoichiometric_coeff;
};
#endif // REACTANT2BODY_H
