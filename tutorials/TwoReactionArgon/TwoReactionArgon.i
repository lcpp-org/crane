[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]

[Variables]
  [e]
    family = SCALAR
    initial_condition = 1 #cm^-3
  []

  [Ar+]
    family = SCALAR
    initial_condition = 1 #cm^-3
  []

  [Ar]
    family = SCALAR
    initial_condition = 2.5e19 #cm^-3
    scaling = 2.5e-19
  []
[]

[ScalarKernels]
  [de_dt]
    type = ODETimeDerivative
    variable = e
  []

  [dAr+_dt]
    type = ODETimeDerivative
    variable = Ar+
  []

  [dAr_dt]
    type = ODETimeDerivative
    variable = Ar
  []
[]

[AuxVariables]
  [reduced_field]
    family = SCALAR
    initial_condition = 30 #Td
  []
[]

[ChemicalReactions]
  [ScalarNetwork]
    species = 'e Ar+ Ar'
    file_location = 'data'
    sampling_variable = 'reduced_field'
    interpolation_type = 'linear'
    reactions = 'e + Ar -> e + e + Ar+         : EEDF (ionization)
                 e + Ar+ + Ar -> Ar + Ar       : 1e-25'
  []
[]

[Executioner]
  type = Transient
  end_time = 0.75e-6 #s
  solve_type = 'newton'
  dt = 1e-10 #s
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
[]

[Preconditioning]
  active = 'smp'

  [smp]
    type = SMP
    full = true
  []

  [fd]
    type = FDP
    full = true
  []
[]

[Outputs]
  [out]
    type = CSV
  []
[]
