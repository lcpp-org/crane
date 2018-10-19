[GlobalParams]
  use_log = true
[]

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
    initial_condition = 1
  [../]

  [./Ar+]
    initial_condition = 1
  [../]

  [./Ar]
    initial_condition = 2.5e19
  [../]
[]

[Kernels]
  [./de_dt]
    type = TimeDerivativeLog
    variable = e
  [../]

  [./dAr+_dt]
    type = TimeDerivativeLog
    variable = Ar+
  [../]

  [./dAr_dt]
    type = TimeDerivativeLog
    variable = Ar
  [../]
[]

[ChemicalReactions]
  [./ZapdosNetwork]
    species = 'e Ar+ Ar'
    file_location = 'Example1'
    reaction_coefficient_format = 'rate'
    # reactions = 'e + Ar + Ar -> Ar + e + e + e  : 1e-31'
    # reactions = 'e + Ar -> Ar + e + e  : 4e-12'
    # reactions = 'e + Ar -> e  : 100000000'
    reactions = 'e + Ar -> e + e + Ar+          : BOLOS
                 e + Ar+ + Ar -> Ar + Ar        : 1e-25'

   [../]
[]

[AuxVariables]
  [./reduced_field]
    initial_condition = 50e-21
  [../]
[]


[Executioner]
  type = Transient
  end_time = 0.25e-6
  dt = 1e-9
  solve_type = 'linear'
  dtmin = 1e-20
  dtmax = 1e-8
  # petsc_options_iname = '-snes_linesearch_type'
  # petsc_options_value = 'basic'
  # petsc_options_value = 'l2'
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
  # exodus = true
  # [./out]
  #   type = Exodus
  #   execute_on = 'TIMESTEP_END'
  # [../]
  csv = true
  # perf_log = true
  interval = 10
  # [./csv_out]
    # type = CSV
    # show = 'e Ar Ar+'
  # [../]
[]
