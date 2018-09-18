#ifndef PRODUCT3BODYSCALAR_H
#define PRODUCT3BODYSCALAR_H

#include "ODEKernel.h"
// #include "RateCoefficientProvider.h"

class Product3BodyScalar;

template <>
InputParameters validParams<Product3BodyScalar>();

class Product3BodyScalar : public ODEKernel
{
public:
  Product3BodyScalar(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  int _v_var;
  const VariableValue & _v;
  int _w_var;
  const VariableValue & _w;
  int _x_var;
  const VariableValue & _x;
  const VariableValue & _rate_coefficient;

  Real _n_gas;
  Real _stoichiometric_coeff;
  // Real _reaction_coeff;
  bool _v_eq_u;
  bool _w_eq_u;
  bool _x_eq_u;
  bool _rate_constant_equation;

  // const RateCoefficientProvider & _data;
};

#endif /* PRODUCT3BODYSCALAR_H */
