[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]

[Variables]
  # ODE variables
  [./y1]
    family = SCALAR
    order = FIRST
    initial_condition = 0.4
  [../]

  [./y2]
    family = SCALAR
    order = FIRST
    initial_condition = 0.6
  [../]

  [./y3]
    family = SCALAR
    order = FIRST
    initial_condition = 0.0
  [../]
[]

[ScalarKernels]
  [./dy1_dt]
    type = ODETimeDerivative
    variable = y1
  [../]

  [./dy2_dt]
    type = ODETimeDerivative
    variable = y2
  [../]

  [./dy3_dt]
    type = ODETimeDerivative
    variable = y3
  [../]
[]

# Stiff
[ChemicalReactions]
  [./ScalarNetwork]
    species = 'y1 y2 y3'
    reaction_coefficient_format = 'rate'

    reactions = 'y1 -> y2          : 2.7e10
                 y1 -> y3          : 9.0e8
                 y2 -> y1          : 3.8e1
                 y3 -> y1          : 1.7e1
                 y2 -> y3          : 1.0e6
                 y3 -> y2          : 7.5e4'
  [../]
[]

# Nonstiff
# [ChemicalReactions]
#   [./ScalarNetwork]
#     species = 'y1 y2 y3'
#     reaction_coefficient_format = 'rate'
#
#     reactions = 'y1 -> y2          : 9e1
#                  y1 -> y3          : 1e2
#                  y2 -> y1          : 1e1
#                  y3 -> y1          : 2e1
#                  y2 -> y3          : 5e1
#                  y3 -> y2          : 3e1'
#   [../]
# []


[Executioner]
  type = Transient
  end_time = 1e-1
  solve_type = NEWTON
  dtmin = 1e-20

  steady_state_detection = true
  steady_state_tolerance = 1e-06
  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.9
    dt = 1e-8
    growth_factor = 1.01
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Outputs]
  csv = true
  interval = 10
[]
