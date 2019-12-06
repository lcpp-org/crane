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

#ifndef PRODUCTIONFIRSTORDER_H
#define PRODUCTIONFIRSTORDER_H

#include "Kernel.h"

// Forward Declaration
class ProductionFirstOrder;

template <>
InputParameters validParams<ProductionFirstOrder>();

class ProductionFirstOrder : public AuxScalarKernel
{
public:
  ProductionFirstOrder(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  // MooseVariable & _coupled_var_A;
  // MooseVariable & _coupled_var_B;
  const VariableValue & _v;

  // The reaction coefficient
  const VariableValue & _rate_coefficient;
  Real _stoichiometric_coeff;
};
#endif // PRODUCTIONFIRSTORDER_H
