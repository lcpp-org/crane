#include "ADEEDFReactionTownsendLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFReactionTownsendLog);

InputParameters
ADEEDFReactionTownsendLog::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addRequiredCoupledVar("potential", "The potential.");
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredCoupledVar("target",
                               "The (heavy species) target of the electron-impact reaction.");
  params.addRequiredParam<Real>(
      "coefficient",
      "The number of species consumed or produced in this reaction.\ne.g. e + Ar -> e + e + Arp:\n "
      "coefficient of e is 1, coefficient of Ar is -1, and coefficient of Arp is 1.");
  params.addRequiredParam<Real>("position_units", "Units of position.");
  params.addRequiredParam<std::string>("reaction", "Stores the full reaction equation.");
  params.addParam<std::string>(
      "number",
      "",
      "The reaction number. Optional, just for material property naming purposes. If a single "
      "reaction has multiple different rate coefficients (frequently the case when multiple "
      "species are lumped together to simplify a reaction network), this will prevent the same "
      "material property from being declared multiple times.");
  return params;
}

ADEEDFReactionTownsendLog::ADEEDFReactionTownsendLog(
    const InputParameters & parameters)
  : ADKernel(parameters),
    _r_units(1. / getParam<Real>("position_units")),
    _diffem(getADMaterialProperty<Real>("diffem")),
    _muem(getADMaterialProperty<Real>("muem")),
    _alpha(getADMaterialProperty<Real>("alpha" + getParam<std::string>("number") + "_" +
                                       getParam<std::string>("reaction"))),
    //_mean_en(adCoupledValue("mean_en")),
    _grad_potential(adCoupledGradient("potential")),
    _em(adCoupledValue("electrons")),
    _target(adCoupledValue("target")),
    _coefficient(getParam<Real>("coefficient")),
    _grad_em(adCoupledGradient("electrons"))
//_target(isCoupled("target") ? adCoupledValue("target") : _em),
{
}

ADReal
ADEEDFReactionTownsendLog::computeQpResidual()
{
  return -_test[_i][_qp] * _alpha[_qp] * std::exp(_target[_qp]) *
         (-_muem[_qp] * -_grad_potential[_qp] * _r_units * std::exp(_em[_qp]) -
          _diffem[_qp] * std::exp(_em[_qp]) * _grad_em[_qp] * _r_units)
             .norm() *
         _coefficient;
}
