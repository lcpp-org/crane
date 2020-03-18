#include "ADEEDFReactionLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFReactionLog);

template <ComputeStage compute_stage>
InputParameters
ADEEDFReactionLog<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredCoupledVar("target",
                               "The (heavy species) target of the electron-impact reaction.");
  params.addRequiredParam<Real>(
      "coefficient",
      "The number of species consumed or produced in this reaction.\ne.g. e + Ar -> e + e + Arp:\n "
      "coefficient of e is 1, coefficient of Ar is -1, and coefficient of Arp is 1.");
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

template <ComputeStage compute_stage>
ADEEDFReactionLog<compute_stage>::ADEEDFReactionLog(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _reaction_coeff(getADMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                                getParam<std::string>("reaction"))),
    _em(adCoupledValue("electrons")),
    _target(adCoupledValue("target")),
    _coefficient(getParam<Real>("coefficient"))
{
}

template <ComputeStage compute_stage>
ADReal
ADEEDFReactionLog<compute_stage>::computeQpResidual()
{
  return -_test[_i][_qp] * _reaction_coeff[_qp] * std::exp(_em[_qp] + _target[_qp]) * _coefficient;
}
