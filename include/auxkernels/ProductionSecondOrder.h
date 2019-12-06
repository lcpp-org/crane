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

#ifndef PRODUCTIONSECONDORDER_H
#define PRODUCTIONSECONDORDER_H

#include "Kernel.h"

// Forward Declaration
class ProductionSecondOrder;

template <>
InputParameters validParams<ProductionSecondOrder>();

class ProductionSecondOrder : public AuxScalarKernel
{
public:
  ProductionSecondOrder(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  // MooseVariable & _coupled_var_A;
  // MooseVariable & _coupled_var_B;
  const VariableValue & _v;
  const VariableValue & _w;

  // The reaction coefficient
  const VariableValue & _rate_coefficient;
  Real _stoichiometric_coeff;
};
#endif // PRODUCTIONSECONDORDER_H
