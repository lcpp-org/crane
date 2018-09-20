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

#ifndef PRODUCTTHIRDORDERLOG_H
#define PRODUCTTHIRDORDERLOG_H

#include "Kernel.h"

// Forward Declaration
class ProductThirdOrderLog;

template <>
InputParameters validParams<ProductThirdOrderLog>();

class ProductThirdOrderLog : public Kernel
{
public:
  ProductThirdOrderLog(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // MooseVariable & _coupled_var_A;
  // MooseVariable & _coupled_var_B;
  const VariableValue & _v;
  const VariableValue & _w;
  const VariableValue & _x;
  unsigned int _v_id;
  unsigned int _w_id;
  unsigned int _x_id;
  // const MaterialProperty<Real> & _n_gas;

  // The reaction coefficient
  const MaterialProperty<Real> & _reaction_coeff;
  Real _stoichiometric_coeff;
};
#endif // PRODUCTTHIRDORDERLOG_H
