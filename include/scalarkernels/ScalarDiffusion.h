#pragma once

#include "ODEKernel.h"
#include "RateCoefficientProvider.h"

class ScalarDiffusion : public ODEKernel
{
public:
  ScalarDiffusion(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  Real computeQpResidual() override;
  Real computeQpJacobian() override;
  Real computeQpOffDiagJacobian(unsigned int) override;

  Real _rate;
};
