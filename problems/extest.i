[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
[]

[Variables]
  [./Ar]
    family = SCALAR
    order = FIRST
    initial_condition = 2.5e19
    scaling = 2.5e-19
  [../]

  [./Ar+]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]

  [./e]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]
[]

[ScalarKernels]
  [./de_dt]
    type = ODETimeDerivative
    variable = e
  [../]

  [./dAr_dt]
    type = ODETimeDerivative
    variable = Ar
  [../]

  [./dAr+_dt]
    type = ODETimeDerivative
    variable = Ar+
  [../]
[]

[ChemicalReactions]
  [./ScalarNetwork]
    species = 'e Ar Ar+'
    file_location = 'Example1'
    reactions = 'e + Ar -> e + e + Ar+  : BOLOS
                 e + Ar+ + Ar -> Ar + Ar : 1e-25'
  [../]
[]

[AuxVariables]
  [./reduced_field]
    family = SCALAR
    order = FIRST
    initial_condition = 50e-21
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  end_time = 0.25e-6
  dt = 1e-10
  solve_type = NEWTON
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
[]

[Outputs]
  csv = true
[]
