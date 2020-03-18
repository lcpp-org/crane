#include "ADEEDFEnergyLog.h"

// MOOSE includes
#include "MooseUtils.h"
#include "MooseVariable.h"

registerADMooseObject("CraneApp", ADEEDFEnergyLog);

template <ComputeStage compute_stage>
InputParameters
ADEEDFEnergyLog<compute_stage>::validParams()
{
  InputParameters params = ADKernel<compute_stage>::validParams();
  params.addRequiredCoupledVar("electrons", "The electron density.");
  params.addRequiredCoupledVar("target", "The target species.");
  params.addRequiredParam<Real>("threshold_energy", "Energy required for reaction to take place.");
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
ADEEDFEnergyLog<compute_stage>::ADEEDFEnergyLog(const InputParameters & parameters)
  : ADKernel<compute_stage>(parameters),
    _reaction_name(getParam<std::string>("reaction")),
    _reaction_coefficient(getADMaterialProperty<Real>("k" + getParam<std::string>("number") + "_" +
                                                      getParam<std::string>("reaction"))),
    _em(adCoupledValue("electrons")),
    _target(adCoupledValue("target")),
    _threshold_energy(getParam<Real>("threshold_energy"))
{
}

template <ComputeStage compute_stage>
ADReal
ADEEDFEnergyLog<compute_stage>::computeQpResidual()
{
  return -_test[_i][_qp] * _reaction_coefficient[_qp] * std::exp(_em[_qp] + _target[_qp]) *
         _threshold_energy;
}
