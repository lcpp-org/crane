#include "ProductSecondOrderMoles.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductSecondOrderMoles);

template <>
InputParameters
validParams<ProductSecondOrderMoles>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ProductSecondOrderMoles::ProductSecondOrderMoles(const InputParameters & parameters)
  : Kernel(parameters),

    // _coupled_var_A(*getVar("v", 0)),
    // _coupled_var_B(*getVar("w", 0)),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _v_id(coupled("v") ? coupled("w") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    // _reaction_coeff(getMaterialProperty<Real>("k" + _coupled_var_A.name() + _coupled_var_B.name())),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ProductSecondOrderMoles::computeQpResidual()
{
  if (isCoupled("w") && isCoupled("v"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_v[_qp]) * std::exp(_w[_qp]);
  }
  else if (isCoupled("v") && !isCoupled("w"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_v[_qp]) * _n_gas[_qp];
  }
  else if (isCoupled("w") && !isCoupled("v"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_w[_qp]) * _n_gas[_qp];
  }
  else
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * _n_gas[_qp] * _n_gas[_qp];
  }
}

Real
ProductSecondOrderMoles::computeQpJacobian()
{
  return 0.0;
}

Real
ProductSecondOrderMoles::computeQpOffDiagJacobian(unsigned int jvar)
{
  // This jacobian is incorrect, I think. How to fix? - S. Keniley, 4/19/2018
  if (isCoupled("w") && isCoupled("v"))
  {
    if (_v_id != _w_id)
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
                std::exp(_v[_qp]) * std::exp(_w[_qp]) * _phi[_j][_qp];
      }
      else if (jvar == _w_id)
      {
        return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
                std::exp(_v[_qp]) * std::exp(_w[_qp]) * _phi[_j][_qp];
      }
      else
        return 0.0;
    }
    else
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * 2.0 * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
                std::exp(_v[_qp]) * std::exp(_w[_qp]) * _phi[_j][_qp];
      }
      else
      {
        return 0.0;
      }
    }
  }
  else if (!isCoupled("w") && isCoupled("v"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
              std::exp(_v[_qp]) * _n_gas[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else if (isCoupled("w") && !isCoupled("v"))
  {
    if (jvar == _w_id)
      return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
            std::exp(_w[_qp]) * _n_gas[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
