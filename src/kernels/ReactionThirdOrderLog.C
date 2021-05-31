#include "ReactionThirdOrderLog.h"

using MetaPhysicL::raw_value;

registerMooseObject("CraneApp", ReactionThirdOrderLog);
registerMooseObject("CraneApp", ADReactionThirdOrderLog);

template <bool is_ad>
InputParameters
ReactionThirdOrderLogTempl<is_ad>::validParams()
{
  InputParameters params = Kernel::validParams();
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

template <bool is_ad>
ReactionThirdOrderLogTempl<is_ad>::ReactionThirdOrderLogTempl(const InputParameters & parameters)
  : GenericKernel<is_ad>(parameters),
    _v(this->template coupledGenericValue<is_ad>("v")),
    _w(this->template coupledGenericValue<is_ad>("w")),
    _x(this->template coupledGenericValue<is_ad>("x")),
    _v_id(coupled("v")),
    _w_id(coupled("w")),
    _x_id(coupled("x")),
    _v_eq_u(this->template getParam<bool>("_v_eq_u")),
    _w_eq_u(this->template getParam<bool>("_w_eq_u")),
    _x_eq_u(this->template getParam<bool>("_x_eq_u")),
    _reaction_coeff(this->template getGenericMaterialProperty<Real, is_ad>(
        "k" + this->template getParam<std::string>("number") + "_" +
        this->template getParam<std::string>("reaction"))),
    _stoichiometric_coeff(this->template getParam<Real>("coefficient"))
{
}

template <bool is_ad>
GenericReal<is_ad>
ReactionThirdOrderLogTempl<is_ad>::computeQpResidual()
{
  return -_test[_i][_qp] * _stoichiometric_coeff * _reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp] + _x[_qp]);
}

template <bool is_ad>
Real
ReactionThirdOrderLogTempl<is_ad>::computeQpJacobian()
{
  Real power;

  power = 0;
  if (_v_eq_u)
    power += 1;
  if (_w_eq_u)
    power += 1;
  if (_x_eq_u)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * raw_value(_reaction_coeff[_qp] * power *
         std::exp(_v[_qp] + _w[_qp] + _x[_qp])) * _phi[_j][_qp];
}

template <bool is_ad>
Real
ReactionThirdOrderLogTempl<is_ad>::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real power;
  power = 0;

  if (!_v_eq_u && jvar == _v_id)
    power += 1;

  if (!_w_eq_u && jvar == _w_id)
    power += 1;

  if (!_x_eq_u && jvar == _x_id)
    power += 1;

  return -_test[_i][_qp] * _stoichiometric_coeff * raw_value(_reaction_coeff[_qp] *
         std::exp(_v[_qp] + _w[_qp] + _x[_qp])) * power * _phi[_j][_qp];
}

template class ReactionThirdOrderLogTempl<false>;
template class ReactionThirdOrderLogTempl<true>;
