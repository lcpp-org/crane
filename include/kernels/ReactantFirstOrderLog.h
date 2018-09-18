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

#ifndef REACTANTFIRSTORDERLOG_H
#define REACTANTFIRSTORDERLOG_H

#include "Kernel.h"

// Forward Declaration
class ReactantFirstOrderLog;

template <>
InputParameters validParams<ReactantFirstOrderLog>();

class ReactantFirstOrderLog : public Kernel
{
public:
  ReactantFirstOrderLog(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  // The reaction coefficient
  // MooseVariable & _coupled_var_A;
  // const MaterialProperty<Real> & _reaction_coeff;
  const MaterialProperty<Real> & _diff_rate;
  // const MaterialProperty<Real> & _n_gas;
  Real _stoichiometric_coeff;

};
#endif // REACTANTFIRSTORDERLOG_H
