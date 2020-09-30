# MODEL DESCRIPTION:
#
# This model is based on a simplified hydrogen collisional radiative model 
# found in Rehman's PhD thesis:
#
#   Rehman, T. "Studies on plasma-chemical reduction", PhD, Technische 
#     Universiteit Eindhoven (2018)
#     URL:  www.tue.nl/taverne
#
#     Section 3.3, pp. 24-29

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]

[Variables]
  # ODE variables
  [H]
    family = SCALAR
    order = FIRST
    initial_condition = 0.4
  []

  [H*]
    family = SCALAR
    order = FIRST
    initial_condition = 0.6
  []

  [H+]
    family = SCALAR
    order = FIRST
    initial_condition = 0.0
  []
[]

[ScalarKernels]
  [dyH_dt]
    type = ODETimeDerivative
    variable = H
  []

  [dyH*_dt]
    type = ODETimeDerivative
    variable = H*
  []

  [dyH+_dt]
    type = ODETimeDerivative
    variable = H+
  []
[]

# Stiff
[ChemicalReactions]
  [ScalarNetwork]
    species = 'H H* H+'

    # Note that these are transition frequencies, not really "reactions" 
    # in the traditional sense
    reactions = 'H -> H*          : 2.7e10
                 H -> H+          : 9.0e8
                 H* -> H          : 3.8e1
                 H+ -> H          : 1.7e1
                 H* -> H+         : 1.0e6
                 H+ -> H*         : 7.5e4'
  []
[]

[Executioner]
  type = Transient
  end_time = 1e-1
  #solve_type = NEWTON
  solve_type = LINEAR
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
  # Prints CSV output every 10 timesteps. 
  csv = true
  interval = 10

  # This option prevents all of the scalar variables from being printed
  # each timestep. Not necessary, but definitely recommended. 
  [Console]
    type = Console
    execute_scalars_on = none 
  []
[]
