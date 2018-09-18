[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]


[Variables]
  # ODE variables
  [./e]
    family = SCALAR
    order = FIRST
    initial_condition = 1e6
    scaling = 1e-6
  [../]

  [./Ar+]
    family = SCALAR
    order = FIRST
    initial_condition = 1e6
    scaling = 1e-6
  [../]

  [./Ar]
    family = SCALAR
    order = FIRST
    initial_condition = 3.219e18
    scaling = 1e-18
  [../]

  [./Ar*]
    family = SCALAR
    order = FIRST
    initial_condition = 1e6
    scaling = 1e-6
  [../]

  [./Ar2+]
    family = SCALAR
    order = FIRST
    initial_condition = 100
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

  [./dAr*_dt]
    type = ODETimeDerivative
    variable = Ar*
  [../]

  [./dAr2+_dt]
    type = ODETimeDerivative
    variable = Ar2+
  [../]
[]


[ChemicalReactions]
  species = 'e Ar* Ar+ Ar Ar2+'
  reaction_coefficient_format = 'rate'
  include_electrons = true
  electron_density = e
  file_location = 'OutputRates_Crane_ex2'
  scalar_problem = true
  equation_constants = 'Tgas Te'
  equation_values = '300 50000'

  # Reaction coefficients in [s^-1], [cm^3/s], [cm^6/s]
  reactions = 'e + Ar -> e + e + Ar+          : BOLOS
               e + Ar -> Ar* + e              : BOLOS
               e + Ar* -> Ar + e              : 2.80566e-8
               e + Ar* -> Ar+ + e + e         : BOLOS
               Ar2+ + e -> Ar* + Ar           : 2.75918e-8
               Ar2^+ + Ar -> Ar^+ + Ar + Ar   : 6.526e-30
               Ar* + Ar* -> Ar2+ + e          : 6.0e-10
               Ar+ + e + e -> Ar + e          : 1.22096e-29
               Ar* + Ar + Ar -> Ar + Ar + Ar  : 1.399e-32
               Ar+ + Ar + Ar -> Ar2+ + Ar     : 2.25e-31
               e -> W                         : 5348.91
               Ar+ -> W                       : 5348.91
               Ar2+ -> W                      : 5348.91'

# I'd like to include a reaction with rate coefficients like this:
# reactions = 'Ar2+ + Ar -> Ar+ + e + e     : {6.06e-6/Tgas*exp(-15130.0/Tgas)}'

  reaction_coefficient = '7.767e-20'
[]

[Executioner]
  type = Transient
  end_time = 1e-3
  solve_type = 'newton'
  # scheme = bdf2
  dtmin = 1e-20
  dtmax = 1e-5
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.9
    dt = 1e-11
    growth_factor = 1.001
  [../]
[]

[Preconditioning]
  active = 'smp'

  [./smp]
    type = SMP
    full = true
  [../]

  [./fd]
    type = FDP
    full = true
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
    show = 'Ar Ar* Ar+ Ar2+ e'
  [../]
[]
