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

#ifndef REACTANTARXN_H
#define REACTANTARXN_H

#include "NodalKernel.h"

// Forward Declaration
class ReactantARxn;

template <>
InputParameters validParams<ReactantARxn>();

class ReactantARxn : public NodalKernel
{
public:
  ReactantARxn(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  // The reaction coefficient
  Real _rate_coefficient;
  Real _stoichiometric_coeff;
};
#endif // REACTANTARXN_H
