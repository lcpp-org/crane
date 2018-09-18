[GlobalParams]
  #use_log = true
  # execute_on = linear
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
  # nx = 20
  # elem_type = EDGE2
[]

[Variables]
  # ODE variables
  [./e]
    family = SCALAR
    order = FIRST
    initial_condition = 1
    scaling = 1e-6
  [../]

  [./Ar+]
    family = SCALAR
    order = FIRST
    initial_condition = 1
    scaling = 1e-6
  [../]

  [./Ar]
    family = SCALAR
    order = FIRST
    #initial_condition = 3.219e18
    initial_condition = 2.5e19
    scaling = 1e-19
  [../]
[]

[ScalarKernels]
  [./de_dt]
    type = ODETimeDerivative
    variable = e
  [../]

  [./dAr+_dt]
    type = ODETimeDerivative
    variable = Ar+
  [../]

  [./dAr_dt]
    type = ODETimeDerivative
    variable = Ar
  [../]
[]

[ChemicalReactions]
  species = 'e Ar+ Ar'
  reaction_coefficient_format = 'rate'
  include_electrons = true
  electron_density = e
  # file_location = 'OutputRates_Crane'
  file_location = 'OutputRates_Crane_ex2'
  scalar_problem = true

  # Reaction coefficients in [s^-1], [cm^3/s], [cm^6/s]
  reactions = 'e + Ar -> e + e + Ar+          : BOLOS
               e + Ar+ + Ar -> Ar + Ar       : 1e-25'

  # reaction_coefficient = '7.767e-20'
[]


# [UserObjects]
#   [./rate_provider]
#     type = RateCoefficientProvider
#     # rate_format = 'Constant'
#     # rate_constant = 6.0e-16
#     rate_format = 'EEDF'
#     # file_location = 'OutputRates_Crane'
#     property_file = 'OutputRates_Crane_ex2/reaction_e + Ar -> e + e + Ar+.txt'
#     # property_file = 'OutputRates_Crane/reaction_e + Ar -> Ar* + e.txt'
#     # property_file = 'OutputRates_Crane/reaction_e + Ar* -> Ar+ + e + e.txt'
#   [../]
# []

[Executioner]
  # implicit-euler,  explicit-euler,  crank-nicolson,  bdf2,  explicit-midpoint,  dirk,  explicit-tvd-rk-2
  # PJFNK JFNK NEWTON FD LINEAR
  #line_search = none
  type = Transient
  # end_time = 1.17e-4
  end_time = 0.25e-6
  # dt = 1.0e-18
  # dt = 1e-10
  dt = 5e-11
  # num_steps  = 1
  solve_type = 'newton'
  # scheme = bdf2
  dtmin = 1e-20
  dtmax = 1e-8
  petsc_options_iname = '-snes_linesearch_type'
  # petsc_options_value = 'basic'
  petsc_options_value = 'l2'
  # [./TimeStepper]
  #   type = IterationAdaptiveDT
  #   cutback_factor = 0.9
  #   dt = 1e-11
  #   growth_factor = 1.001
  #   # optimal_iterations = 20
  # [../]
[]

[Preconditioning]
  active = 'smp'

  [./smp]
    type = SMP
    full = true
    #ksp_norm = none
  [../]

  [./fd]
    type = FDP
    full = true

    #petsc_options_iname = '-pc_type'
    #petsc_options_value = 'lu'
  [../]
[]

[Outputs]
  #exodus = true
  csv = true
  perf_log = true
  interval = 100
  [./csv_out]
    type = CSV
    #execute_on = 'final'
    show = 'e Ar Ar+'
  [../]
[]
