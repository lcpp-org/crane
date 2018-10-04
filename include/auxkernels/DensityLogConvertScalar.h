#ifndef DENSITYLOGCONVERTSCALAR_H
#define DENSITYLOGCONVERTSCALAR_H

#include "AuxScalarKernel.h"

class DensityLogConvertScalar;

template <>
InputParameters validParams<DensityLogConvertScalar>();

class DensityLogConvertScalar : public AuxScalarKernel
{
public:
  DensityLogConvertScalar(const InputParameters & parameters);

  virtual ~DensityLogConvertScalar() {}

protected:
  virtual Real computeValue() override;

  const VariableValue & _density_log;
  bool _convert_moles;
  // const MaterialProperty<Real> & _N_A;
};

#endif // DENSITYLOGCONVERTSCALAR_H
