[GlobalParams]
  #use_log = true
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
    initial_condition = 3.1065548e-13
  [../]

  [./Ar+]
    family = SCALAR
    order = FIRST
    initial_condition = 3.1065548e-13
  [../]

  [./Ar]
    family = SCALAR
    order = FIRST
    initial_condition = 1.0
    scaling = 1e-13
  [../]

  [./Ar*]
    family = SCALAR
    order = FIRST
    initial_condition = 3.1065548e-13
    #scaling = 1e12
  [../]

  # [./Ar2+]
  #   family = SCALAR
  #   order = FIRST
  #   initial_condition = 100
  # [../]
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

  [./dAr*_dt]
    type = ODETimeDerivative
    variable = Ar*
  [../]

  # [./dAr2+_dt]
  #   type = ODETimeDerivative
  #   variable = Ar2+
  # [../]

  # [./e_source]
  #   type = Product2BodyScalar
  #   variable = e
  #   v = e
  #   v_eq_u = true
  #   n_gas = 3.219e24
  #   coefficient = 1
  #   reaction_coefficient = 1e-17
  #   rate_provider = rate_provider
  # [../]
  #
  # [./Ar+_source]
  #   type = Product2BodyScalar
  #   variable = Ar+
  #   v = e
  #   n_gas = 3.219e24
  #   coefficient = 1
  #   reaction_coefficient = 1e-17
  #   rate_provider = rate_provider
  # [../]

  # [./Ar*_source]
  #  type = Product2BodyScalar
  #  variable = Ar*
  #  # v = e
  #  n_gas = 3.219e24
  #  coefficient = 1
  #  # reaction_coefficient = 3.4481e-16
  #  rate_provider = rate_provider
  # [../]

  # [./e_diffusion]
  #   type = ScalarDiffusion
  #   variable = e
  #   rate = 53000
  # [../]
  #
  # [./Ar+_diffusion]
  #   type = ScalarDiffusion
  #   variable = Ar+
  #   rate = 53000
  # [../]

  # [./Ar2+_diffusion]
  #   type = ScalarDiffusion
  #   variable = Ar2+
  #   rate = 5300
  # [../]

  # [./Ar_diffusion]
  #   type = ScalarDiffusion
  #   variable = Ar
  #   rate = 5300
  # [../]

  # [./e_test]
  #   type = Product2BodyScalar
  #   variable = e
  #   v = e
  #   # w = Ar
  #   v_eq_u = true
  #   n_gas = 3.24e18
  #   coefficient = 1
  #   rate_provider = rate_provider
  # [../]

  [./Ar_test]
    type = Reactant2BodyScalar
    variable = Ar
    v = e
    # v_eq_u = true
    # n_gas = 3.24e18
    n_gas = 1.0
    coefficient = -1
    rate_provider = rate_provider
  [../]

[]

# [ChemicalReactions]
#   ### THESE VALUES ARE GOING TO BE WRONG BECAUSE BOLOS WAS RUN AT THE WRONG
#   ### VALUES (PRESSURE, ETC.)
#   ### NEEDS TO BE RERUN.
#   species = 'e Ar* Ar+ Ar'
#   reaction_coefficient_format = 'rate'
#   include_electrons = true
#   electron_density = e
#   # file_location = 'OutputRates_Crane'
#   file_location = 'OutputRates_Crane_ex2'
#   scalar_problem = true
#
#   # Reaction coefficients in [s^-1], [m^3/s], [m^6/s]
#   # reactions = 'e + Ar -> e + e + Ar+          : BOLOS
#   #              e + Ar -> Ar* + e              : BOLOS
#   #              e + Ar* -> Ar+ + e + e         : BOLOS
#   #              Ar* + Ar + Ar -> Ar + Ar + Ar  : 1.4e-41
#   #              Ar+ + e + e -> Ar + e          : 1.229e-38
#   #              Ar* + Ar* -> Ar2+ + e          : 6.0e-16
#   #              Ar+ + Ar + Ar -> Ar2+ + Ar     : 2.25e-40
#   #              e + Ar* -> Ar + e              : 1.0e-12'
#
#   # Reaction coefficients in [s^-1], [cm^3/s], [cm^6/s]
#   reactions = 'e + Ar -> e + e + Ar+          : 1e-11'
#   # reactions = 'e + Ar -> e + e + Ar+          : BOLOS
#   #              e + Ar -> Ar* + e              : BOLOS
#   #              e + Ar* -> Ar+ + e + e         : BOLOS
#   #              Ar* + Ar + Ar -> Ar + Ar + Ar  : 1.4e-32
#   #              Ar+ + e + e -> Ar + e          : 1.229e-29
#   #              Ar* + Ar* -> Ar2+ + e          : 6.0e-10
#   #              Ar+ + Ar + Ar -> Ar2+ + Ar     : 2.25e-31
#   #              e + Ar* -> Ar + e              : 1.0e-6
#   #              Ar2+ + e -> Ar* + Ar           : 2.75918e-12
#   #              Ar2+ + Ar -> Ar+ + e + e       : 2.526e-30'
#   # Missing reactions:
#   #
#   # Ar^+ + Ar + Ar  => Ar2^+ + Ar  !   2.25d-31 * (Tgas/300.0d0)**(-0.4d0)
#   reaction_coefficient = '7.767e-20'
# []


[UserObjects]
  [./rate_provider]
    type = RateCoefficientProvider
    # rate_format = 'Constant'
    # rate_constant = 6.0e-16
    rate_format = 'EEDF'
    # file_location = 'OutputRates_Crane'
    property_file = 'OutputRates_Crane_ex2/reaction_e + Ar -> e + e + Ar+.txt'
    # property_file = 'OutputRates_Crane/reaction_e + Ar -> Ar* + e.txt'
    # property_file = 'OutputRates_Crane/reaction_e + Ar* -> Ar+ + e + e.txt'
  [../]
[]

[Executioner]
  # implicit-euler,  explicit-euler,  crank-nicolson,  bdf2,  explicit-midpoint,  dirk,  explicit-tvd-rk-2
  # PJFNK JFNK NEWTON FD LINEAR
  #line_search = none
  type = Transient
  # end_time = 1.0e-4
  # dt = 1.0e-18
  dt = 1e-5
  num_steps  = 100
  solve_type = 'newton'
  scheme = crank-nicolson
  dtmin = 1e-20
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
  # petsc_options_value = 'l2'
  # [./TimeStepper]
  #   type = IterationAdaptiveDT
  #   cutback_factor = 0.9
  #   dt = 1e-11
  #   growth_factor = 1.05
  #   # optimal_iterations = 20
  # [../]
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
#
#     #petsc_options_iname = '-pc_type'
#     #petsc_options_value = 'lu'
#   [../]
# []

# [Outputs]
#   #exodus = true
#   csv = true
#   perf_log = true
#   [./csv_out]
#     type = CSV
#     #execute_on = 'final'
#     show = 'e Ar* Ar+'
#   [../]
# []
