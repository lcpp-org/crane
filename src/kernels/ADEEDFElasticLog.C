#include "ADEEDFElasticLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFElasticLog);

template <ComputeStage compute_stage>
InputParameters
ADEEDFElasticLog<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredCoupledVar("target", "The target species variable.");
  params.addRequiredParam<std::string>("reaction",
                                       "Stores the name of the reaction (townsend) coefficient, "
                                       "unique to each individual reaction.");
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
ADEEDFElasticLog<compute_stage>::ADEEDFElasticLog(
    const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _reaction_coefficient(getADMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                                      getParam<std::string>("reaction"))),
    _massGas(getMaterialProperty<Real>("mass" + (*getVar("target", 0)).name())),
    _em(adCoupledValue("electrons")),
    _target(adCoupledValue("target"))
{
  _massem = 9.11e-31;
}

template <ComputeStage compute_stage>
ADReal
ADEEDFElasticLog<compute_stage>::computeQpResidual()
{
  ADReal Eel = -3.0 * _massem / _massGas[_qp] * 2.0 / 3. * std::exp(_u[_qp] - _em[_qp]);

  return -_test[_i][_qp] * _reaction_coefficient[_qp] * std::exp(_em[_qp] + _target[_qp]) * Eel;
}
