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
    initial_condition = 0.0
    # scaling = 1e-6
  [../]

  [./Ar+]
    family = SCALAR
    order = FIRST
    initial_condition = 0.0
    # scaling = 1e-6
  [../]

  [./Ar]
    family = SCALAR
    order = FIRST
    # initial_condition = 2.5e19
    # scaling = 2.5e-19
    initial_condition = 44.66540749876102306153
    # scaling = 44e-1
  [../]
[]

[ScalarKernels]
  [./de_dt]
    type = ODETimeDerivativeLog
    variable = e
  [../]

  [./dAr+_dt]
    type = ODETimeDerivativeLog
    variable = Ar+
  [../]

  [./dAr_dt]
    type = ODETimeDerivativeLog
    variable = Ar
  [../]

  # [./Ar_stabilization]
  #   type = LogStabilizationScalar
  #   variable = Ar
  #   offset = 20
  # [../]

  # [./e_stabilization]
  #   type = LogStabilizationScalar
  #   variable = e
  #   offset = 20
  # [../]
  #
  # [./Ar+_stabilization]
  #   type = LogStabilizationScalar
  #   variable = Ar+
  #   offset =
  # [../]
[]

[ChemicalReactions]
  [./ScalarNetwork]
    species = 'e Ar+ Ar'
    file_location = 'Example1'
    use_log = true
    # reactions = 'e + Ar + Ar -> Ar + e + e + e  : 1e-31'
    # reactions = 'e + Ar -> e  : 100000000'
    # reactions = 'e + Ar+ + Ar -> Ar + Ar : 1e-25'
    reactions = 'e + Ar -> e + e + Ar+          : BOLOS
                 e + Ar+ + Ar -> Ar + Ar        : 1e-25'

   [../]
[]

[AuxVariables]
  [./reduced_field]
    order = FIRST
    family = SCALAR
    initial_condition = 50e-21
  [../]

  [./e_density]
    order = FIRST
    family = SCALAR
    initial_condition = 1
  [../]
[]

[AuxScalarKernels]
  [./e_density_calculate]
    type = DensityLogConvertScalar
    variable = e_density
    use_moles = false
    density_log = e
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
  csv = true
  interval = 10
[]
