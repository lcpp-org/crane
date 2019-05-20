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

#ifndef REACTANTTHIRDORDERLOG_H
#define REACTANTTHIRDORDERLOG_H

#include "Kernel.h"

// Forward Declaration
class ReactantThirdOrderLog;

template <>
InputParameters validParams<ReactantThirdOrderLog>();

class ReactantThirdOrderLog : public Kernel
{
public:
  ReactantThirdOrderLog(const InputParameters & parameters);

protected:
  Real computeQpResidual() override;
  Real computeQpJacobian() override;
  Real computeQpOffDiagJacobian(unsigned int) override;

  // The reaction coefficient
  // MooseVariable & _coupled_var_A;
  const MaterialProperty<Real> & _reaction_coeff;
  const VariableValue & _v;
  const VariableValue & _w;
  unsigned int _v_id;
  unsigned int _w_id;
  bool _v_eq_u;
  bool _w_eq_u;
  bool _v_coupled;
  bool _w_coupled;
  const MaterialProperty<Real> & _n_gas;
  Real _stoichiometric_coeff;
};
#endif // REACTANTTHIRDORDERLOG_H
