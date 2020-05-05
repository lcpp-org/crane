#include "ReactionThirdOrderLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactionThirdOrderLog);

template <>
InputParameters
validParams<ReactionThirdOrderLog>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("v", "The first variable that is reacting.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting.");
  params.addRequiredCoupledVar("x", "The third variable that is reacting.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addParam<bool>("_v_eq_u", false, "If coupled v == u.");
  params.addParam<bool>("_w_eq_u", false, "If coupled w == u.");
  params.addParam<bool>("_x_eq_u", false, "If coupled x == u.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ReactionThirdOrderLog::ReactionThirdOrderLog(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledValue("v")),
    _w(coupledValue("w")),
    _x(coupledValue("x")),
    _v_id(coupled("v")),
    _w_id(coupled("w")),
    _x_id(coupled("x")),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u")),
    _x_eq_u(getParam<bool>("_x_eq_u")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient"))
{
}

Real
ReactionThirdOrderLog::computeQpResidual()
{
  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp] + _x[_qp]);
}

Real
ReactionThirdOrderLog::computeQpJacobian()
{
  Real power;

  power = 0;
  if (_v_eq_u)
    power += 1;
  if (_w_eq_u)
    power += 1;
  if (_x_eq_u)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] * power *
         std::exp(_v[_qp] + _w[_qp] + _x[_qp]) * _phi[_j][_qp];
}

Real
ReactionThirdOrderLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power;
  power = 0;

  if (!_v_eq_u && jvar == _v_id)
    power += 1;

  if (!_w_eq_u && jvar == _w_id)
    power += 1;

  if (!_x_eq_u && jvar == _x_id)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp] + _x[_qp]) * power * _phi[_j][_qp];
}
