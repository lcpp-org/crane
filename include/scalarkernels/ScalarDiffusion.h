#ifndef SCALARDIFFUSION_H
#define SCALARDIFFUSION_H

#include "ODEKernel.h"
#include "RateCoefficientProvider.h"

class ScalarDiffusion;

template <>
InputParameters validParams<ScalarDiffusion>();

class ScalarDiffusion : public ODEKernel
{
public:
  ScalarDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian();

  Real _rate;
};

#endif /* SCALARDIFFUSION_H */
