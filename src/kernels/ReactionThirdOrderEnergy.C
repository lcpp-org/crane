//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReactionThirdOrderEnergy.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactionThirdOrderEnergy);

InputParameters
ReactionThirdOrderEnergy::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting.");
  params.addRequiredCoupledVar("x", "The third variable that is reacting.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("threshold_energy", "The change in enthalpy associated with this reaction.");
  params.addParam<bool>("_v_eq_u", false, "Whether or not v and u are the same variable.");
  params.addParam<bool>("_w_eq_u", false, "Whether or not w and u are the same variable.");
  params.addParam<bool>("_x_eq_u", false, "Whether or not x and u are the same variable.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ReactionThirdOrderEnergy::ReactionThirdOrderEnergy(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledValue("v")),
    _w(coupledValue("w")),
    _x(coupledValue("x")),
    _v_id(coupled("v")),
    _w_id(coupled("w")),
    _x_id(coupled("x")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u")),
    _x_eq_u(getParam<bool>("_x_eq_u"))
{
}

Real
ReactionThirdOrderEnergy::computeQpResidual()
{
  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] * _w[_qp] *
         _x[_qp] * _threshold_energy;
}

Real
ReactionThirdOrderEnergy::computeQpJacobian()
{
  return 0.0;
  /*
  Real aval;
  aval = -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _phi[_j][_qp];

  if (_v_eq_u && _w_eq_u && _x_eq_u)
    return aval * 3.0 * _u[_qp] * _u[_qp];
  else if (_v_eq_u && !_w_eq_u && !_x_eq_u)
    return aval * _w[_qp] * _x[_qp];
  else if (_v_eq_u && _w_eq_u && !_x_eq_u)
    return aval * 2.0 * _u[_qp] * _x[_qp];
  else if (_v_eq_u && !_w_eq_u && _x_eq_u)
    return aval * 2.0 * _u[_qp] * _w[_qp];
  else if (!_v_eq_u && _w_eq_u && _x_eq_u)
    return aval * 2.0 * _u[_qp] * _v[_qp];
  else if (!_v_eq_u && _w_eq_u && !_x_eq_u)
    return aval * _u[_qp] * _v[_qp] * _x[_qp];
  else if (!_v_eq_u && !_w_eq_u && _x_eq_u)
    return aval * _u[_qp] * _v[_qp] * _w[_qp];
  else
    return 0.0;
  */
  /*
  Real mult_1, mult_2, mult_3, power, eq_u_mult, gas_mult;
  power = 0.0;
  eq_u_mult = 1.0;
  gas_mult = 1.0;

  if (_v_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_1;
  }
  else
    gas_mult *= mult_1;

  if (_w_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_2;
  }
  else
    gas_mult *= mult_2;

  if (_x_eq_u)
  {
    power += 1.0;
    eq_u_mult *= mult_3;
  }
  else
    gas_mult *= mult_3;

  if (!_v_eq_u && !_w_eq_u && !_x_eq_u)
    return 0.0;
  else
  {
    return -_test[_i][_qp] * _stoichiometric_coeff * power * std::pow(eq_u_mult, power-1) * gas_mult
  * _phi[_j][_qp];
  }
  */
}

Real
ReactionThirdOrderEnergy::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real aval;
  aval = -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _phi[_j][_qp] * _threshold_energy;

  if (!_v_eq_u && !_w_eq_u && !_x_eq_u)
  {
    if (jvar == _v_id && jvar == _w_id && jvar == _x_id)
      return aval * 3.0 * _v[_qp] * _v[_qp]; // This means that v, w, and x are the same variable.
    else if (jvar == _v_id && jvar != _w_id && jvar != _x_id)
      return aval * _w[_qp] * _x[_qp];
    else if (jvar == _v_id && jvar != _w_id && jvar == _x_id) // v and x are the same
      return aval * 2.0 * _v[_qp] * _w[_qp];
    else if (jvar == _v_id && jvar == _w_id && jvar != _x_id)
      return aval * 2.0 * _v[_qp] * _x[_qp];
    else if (jvar != _v_id && jvar == _w_id && jvar == _x_id)
      return aval * _v[_qp] * 2.0 * _w[_qp];
    else if (jvar != _v_id && jvar != _w_id && jvar == _x_id)
      return aval * _v[_qp] * _w[_qp];
    else if (jvar != _v_id && jvar == _w_id && jvar != _x_id)
      return aval * _v[_qp] * _x[_qp];
    else
      return 0.0;
  }
  else if (!_v_eq_u && _w_eq_u && _x_eq_u)
  {
    if (jvar == _v_id)
      return aval * _w[_qp] * _x[_qp];
    else
      return 0.0;
  }
  else if (!_v_eq_u && _w_eq_u && !_x_eq_u)
  {
    if (jvar == _v_id && jvar == _x_id)
      return aval * 2.0 * _v[_qp] * _w[_qp];
    else if (jvar == _v_id && jvar != _x_id)
      return aval * _x[_qp] * _w[_qp];
    else if (jvar != _v_id && jvar == _x_id)
      return aval * _v[_qp] * _w[_qp];
    else
      return 0.0;
  }
  else if (!_v_eq_u && !_w_eq_u && _x_eq_u)
  {
    if (jvar == _v_id && jvar == _w_id)
      return aval * 2.0 * _v[_qp] * _x[_qp];
    else if (jvar == _v_id && jvar != _w_id)
      return aval * _w[_qp] * _x[_qp];
    else if (jvar != _v_id && jvar == _w_id)
      return aval * _v[_qp] * _x[_qp];
    else
      return 0.0;
  }
  else if (_v_eq_u && !_w_eq_u && !_x_eq_u)
  {
    if (jvar == _w_id && jvar == _x_id)
      return aval * _v[_qp] * 2.0 * _w[_qp];
    else if (jvar == _w_id && jvar != _x_id)
      return aval * _v[_qp] * _x[_qp];
    else if (jvar != _w_id && jvar == _x_id)
      return aval * _v[_qp] * _w[_qp];
    else
      return 0.0;
  }
  else if (_v_eq_u && _w_eq_u && !_x_eq_u)
  {
    if (jvar == _x_id)
      return aval * _v[_qp] * _w[_qp];
    else
      return 0.0;
  }
  else if (_v_eq_u && !_w_eq_u && _x_eq_u)
  {
    if (jvar == _w_id)
      return aval * _v[_qp] * _x[_qp];
    else
      return 0.0;
  }
  else
    return 0.0;

  /*
  // This jacobian is incorrect, I think. How to fix? - S. Keniley, 4/19/2018
  if (isCoupled("w") && isCoupled("v"))
  {
    if (_v_id != _w_id)
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _w[_qp] *
               _phi[_j][_qp];
      }
      else if (jvar == _w_id)
      {
        return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] *
               _phi[_j][_qp];
      }
      else
        return 0.0;
    }
    else
    {
      if (jvar == _v_id)
      {
        return -_test[_i][_qp] * 2.0 * _stoichiometric_coeff * _reaction_coeff[_qp] * _v[_qp] *
               _w[_qp] * _phi[_j][_qp];
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
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] *
             _phi[_j][_qp];
    else
      return 0.0;
  }
  else if (isCoupled("w") && !isCoupled("v"))
  {
    if (jvar == _w_id)
      return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _n_gas[_qp] *
             _phi[_j][_qp];
    else
      return 0.0;
  }
  else
  {
    return 0.0;
  }
  */
}
