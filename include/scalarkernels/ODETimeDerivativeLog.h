#pragma once

#include "ODETimeKernel.h"

class ODETimeDerivativeLog : public ODETimeKernel
{
public:
  ODETimeDerivativeLog(const InputParameters & parameters);

  static InputParameters validParams();

  /* virtual void computeJacobian(); */

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  /* virtual Real computeQpOffDiagJacobian(unsigned int jvar); */

  bool _lumping;
};
