#include "ProductThirdOrderLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductThirdOrderLog);

template <>
InputParameters
validParams<ProductThirdOrderLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting.");
  params.addCoupledVar("w", "The second variable that is reacting.");
  params.addCoupledVar("x", "The third variable that is reacting.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  return params;
}

ProductThirdOrderLog::ProductThirdOrderLog(const InputParameters & parameters)
  : Kernel(parameters),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _x(isCoupled("x") ? coupledValue("x") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    _x_id(isCoupled("x") ? coupled("x") : 0),
    _n_gas(getMaterialProperty<Real>("n_gas")),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ProductThirdOrderLog::computeQpResidual()
{
  if (isCoupled("w") && isCoupled("v") && isCoupled("x"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_v[_qp]) * std::exp(_w[_qp]) * std::exp(_x[_qp]);
  }
  else if (isCoupled("v") && !isCoupled("w") && isCoupled("x"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_v[_qp]) * _n_gas[_qp] * std::exp(_x[_qp]);
  }
  else if (!isCoupled("v") && !isCoupled("w") && isCoupled("x"))
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * std::exp(_w[_qp]) * _n_gas[_qp];
  }
  else
  {
    return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] * _n_gas[_qp] * _n_gas[_qp];
  }
  // Need to add more possibilities
}

Real
ProductThirdOrderLog::computeQpJacobian()
{
  return 0.0;
}

Real
ProductThirdOrderLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  // This jacobian is incorrect, I think. How to fix? - S. Keniley, 4/19/2018
  if (isCoupled("v") && isCoupled("w") && isCoupled("x"))
  {
    if (_v_id != _w_id && _v_id != _x_id && _w_id != _x_id)
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
                std::exp(_w[_qp]) * _phi[_j][_qp];
      }
      else if (jvar == _w_id)
      {
        return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
                std::exp(_v[_qp]) * _phi[_j][_qp];
      }
      else
        return 0.0;
    }
    else if (_v_id != _w_id && _v_id != _x_id && _w_id == _x_id)
    {
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
        std::exp(_v[_qp]) * 2.0 * std::exp(_w[_qp]) * std::exp(_x[_qp]) * _phi[_j][_qp];
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
  else if (!isCoupled("w") && isCoupled("v") && isCoupled("x"))
  {
    if (jvar == _v_id)
      return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
              _n_gas[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else if (isCoupled("w") && !isCoupled("v") && isCoupled("x"))
  {
    if (jvar == _w_id)
      return -_test[_i][_qp] * (_stoichiometric_coeff) * _reaction_coeff[_qp] *
            _n_gas[_qp] * _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
}
