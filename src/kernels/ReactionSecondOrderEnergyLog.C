#include "ReactionSecondOrderEnergyLog.h"

// MOOSE includes
#include "MooseVariable.h"

registerMooseObject("CraneApp", ReactionSecondOrderEnergyLog);

InputParameters
ReactionSecondOrderEnergyLog::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("v", "The first variable that is reacting to create u.");
  params.addRequiredCoupledVar("w", "The second variable that is reacting to create u.");
  params.addRequiredParam<std::string>("reaction", "The full reaction equation.");
  params.addRequiredParam<Real>("coefficient", "The stoichiometric coeffient.");
  params.addRequiredParam<Real>("threshold_energy",
                                "The change in enthalpy associated with this reaction.");
  params.addParam<bool>("_v_eq_u", false, "If v == u.");
  params.addParam<bool>("_w_eq_u", false, "If w == u.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ReactionSecondOrderEnergyLog::ReactionSecondOrderEnergyLog(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledValue("v")),
    _w(coupledValue("w")),
    _v_id(coupled("v")),
    _w_id(coupled("w")),
    _reaction_coeff(getMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                              getParam<std::string>("reaction"))),
    _stoichiometric_coeff(getParam<Real>("coefficient")),
    _threshold_energy(getParam<Real>("threshold_energy")),
    _v_eq_u(getParam<bool>("_v_eq_u")),
    _w_eq_u(getParam<bool>("_w_eq_u"))
{
}

Real
ReactionSecondOrderEnergyLog::computeQpResidual()
{
  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp]) * _threshold_energy;
}

Real
ReactionSecondOrderEnergyLog::computeQpJacobian()
{
  return 0.0;
}

Real
ReactionSecondOrderEnergyLog::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power;
  power = 0;

  if (!_v_eq_u && jvar == _v_id)
    power += 1;

  if (!_w_eq_u && jvar == _w_id)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp]) * power * _phi[_j][_qp] * _threshold_energy;
}
