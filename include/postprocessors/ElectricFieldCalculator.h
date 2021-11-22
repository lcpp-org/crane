#pragma once

// MOOSE includes
#include "GeneralPostprocessor.h"

/**
 * This postprocessor computes a side integral of the mass flux.
 */
class ElectricFieldCalculator : public GeneralPostprocessor
{
public:
  ElectricFieldCalculator(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void initialize() override {};
  virtual void execute() override {};
  virtual Real getValue() override;

protected:
  // virtual Real reducedField();

  const VariableValue & _gas_density;
  const VariableValue & _electron_density;

};
