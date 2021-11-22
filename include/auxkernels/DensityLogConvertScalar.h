#pragma once

#include "AuxScalarKernel.h"

class DensityLogConvertScalar : public AuxScalarKernel
{
public:
  DensityLogConvertScalar(const InputParameters & parameters);

  static InputParameters validParams();

  virtual ~DensityLogConvertScalar() {}

protected:
  virtual Real computeValue() override;

  const VariableValue & _density_log;
  bool _convert_moles;
  // const MaterialProperty<Real> & _N_A;
};
