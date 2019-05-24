#include "ProductSecondOrderLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ProductSecondOrderLog);

template <>
InputParameters
validParams<ProductSecondOrderLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addCoupledVar("v", "The first variable that is reacting to create u.");
  params.addCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "If v == u.");
  params.addParam<bool>("_w_eq_u", false, "If w == u.");
  return params;
}

ProductSecondOrderLog::ProductSecondOrderLog(const InputParameters & parameters)
  : Kernel(parameters),
    _v(isCoupled("v") ? coupledValue("v") : _zero),
    _w(isCoupled("w") ? coupledValue("w") : _zero),
    _v_id(isCoupled("v") ? coupled("v") : 0),
    _w_id(isCoupled("w") ? coupled("w") : 0),
    // _n_gas(getMaterialProperty<Real>("n_gas")),
    _reaction_coeff(getMaterialProperty<Real>("k_" + getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u")),
    _v_coupled(isCoupled("v") ? true : false),
    _w_coupled(isCoupled("w") ? true : false)
{
  if (!isCoupled("v") || !isCoupled("w"))
  {
    mooseError("Missing coupled variable(s) in reaction {" + getParam<std::string>("reaction") +
               "}! \nMake sure all reactants are either nonlinear or auxiliary variables.");
  }
}

Real
ProductSecondOrderLog::computeQpResidual()
{
  Real mult1, mult2;
  // if (_v_coupled)
  mult1 = std::exp(_v[_qp]);
  // else
  //{
  //  mult1 = _n_gas[_qp];
  //}

  // if (_w_coupled)
  mult2 = std::exp(_w[_qp]);
  // else
  //{
  //  mult2 = _n_gas[_qp];
  //}

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * mult1 * mult2;
}

Real
ProductSecondOrderLog::computeQpJacobian()
{
  Real mult1, mult2, power, gas_mult;
  power = 0.0;
  gas_mult = 1.0;

  // if (_v_coupled)
  mult1 = std::exp(_v[_qp]);
  // else
  //  mult1 = _n_gas[_qp];

  // if (_w_coupled)
  mult2 = std::exp(_w[_qp]);
  // else
  //  mult2 = _n_gas[_qp];

  if (_v_coupled && _v_eq_u)
    power += 1.0;

  if (_w_coupled && _w_eq_u)
    power += 1.0;

  gas_mult *= mult1 * mult2;

  if (!_v_eq_u && !_w_eq_u)
  {
    return 0.0;
  }
  else
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * power * _reaction_coeff[_qp] * gas_mult *
           _phi[_j][_qp];
  }
}

Real
ProductSecondOrderLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power, mult1, mult2;
  Real gas_mult;
  // Real rate_constant;
  power = 0;
  gas_mult = 1;
  // if (_v_coupled)
  mult1 = std::exp(_v[_qp]);
  // else
  //  mult1 = _n_gas[_qp];

  // if (_w_coupled)
  mult2 = std::exp(_w[_qp]);
  // else
  //  mult2 = _n_gas[_qp];

  if (_v_coupled && !_v_eq_u && jvar == _v_id)
    power += 1;

  if (_w_coupled && !_w_eq_u && jvar == _w_id)
    power += 1;

  gas_mult = mult1 * mult2;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * gas_mult * power *
         _phi[_j][_qp];
}
