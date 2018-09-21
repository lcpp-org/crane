[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]

[Variables]
  # ODE variables
  [./x]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]

  [./y]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]
[]

[ScalarKernels]
  # Time derivatives
  [./dx_dt]
    type = ODETimeDerivative
    variable = x
  [../]
  [./dy_dt]
    type = ODETimeDerivative
    variable = y
  [../]
[]

[ChemicalReactions]
  [./ScalarNetwork]
  species = 'x y'
  reactions = 'x -> x + x             : 0.666667
               x + y -> y             : 1.333333
               y + x -> x + y + y     : 1
               y -> z                 : 1'
  [../]
[]

# [Preconditioning]
#   active = 'smp'
#
#   [./smp]
#     type = SMP
#     full = true
#     #ksp_norm = none
#   [../]
#
#   [./fd]
#     type = FDP
#     full = true
#   [../]
# []

[Executioner]
  type = Transient
  start_time = 0
  end_time = 50
  dt = 0.01
  solve_type = 'newton'
  # petsc_options_iname = '-snes_linesearch_type'
  # petsc_options_value = 'basic'
  # scheme = crank-nicolson
  # timestep_tolerance = 1e-6
[]

[Outputs]
  # [./out]
  #   type = Exodus
  #   execute_on = 'TIMESTEP_END'
  # [../]
  csv = true
  perf_log = true
  [./csv_out]
    type = CSV
    show = 'x y'
  [../]
[]
