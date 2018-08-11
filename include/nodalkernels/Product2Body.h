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

#ifndef PRODUCT2BODY_H
#define PRODUCT2BODY_H

#include "NodalKernel.h"

// Forward Declaration
class Product2Body;

template <>
InputParameters validParams<Product2Body>();

class Product2Body : public NodalKernel
{
public:
  Product2Body(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  MooseVariable & _coupled_var_A;
  const VariableValue & _v;
  MooseVariable & _coupled_var_B;
  const VariableValue & _w;

  // The reaction coefficient
  Real _rate_coefficient;
  Real _stoichiometric_coeff;
};
#endif // PRODUCT2BODY_H
