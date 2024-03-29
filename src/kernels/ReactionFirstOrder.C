//* This file is part of Crane, an open-source
//* application for plasma chemistry and thermochemistry
//* https://github.com/lcpp-org/crane
//*
//* Crane is powered by the MOOSE Framework
//* https://www.mooseframework.org
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ReactionFirstOrder.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactionFirstOrder);

InputParameters
ReactionFirstOrder::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "Whether or not v and u are the same variable.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ReactionFirstOrder::ReactionFirstOrder(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledValue("v")),
    _v_id(coupled("v")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _v_eq_u(getParam<bool>("_v_eq_u"))
{
}

Real
ReactionFirstOrder::computeQpResidual()
{
  return -_test[_i][_qp] * (_stoichiometric_coeff)*_reaction_coeff[_qp] * _v[_qp];
}

Real
ReactionFirstOrder::computeQpJacobian()
{
  if (_v_eq_u)
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _phi[_j][_qp];
  else
    return 0.0;
}

Real
ReactionFirstOrder::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (!_v_eq_u && jvar == _v_id)
    return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * _phi[_j][_qp];
  else
    return 0.0;
}
