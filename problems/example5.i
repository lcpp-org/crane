[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
[]

[Variables]
  [./e]
    family = SCALAR
    order = FIRST
    initial_condition = 1e6
  [../]

  # Add two more scalar variables here
[]

[ScalarKernels]
  [./de_dt]
    type = ODETimeDerivative
    variable = e
  [../]

  # Add two more scalar kernels here

[]

[ChemicalReactions]
  [./ScalarNetwork]
    species = ''
    file_location = 'Example5'
    sampling_variable = 'reduced_field'

    # Add reactions here
    reactions = ''

   [../]
[]

[AuxVariables]
  [./reduced_field]
    order = FIRST
    family = SCALAR
    initial_condition = 50e-21
  [../]
[]


[Executioner]
  type = Transient
  end_time = 0.25e-6
  dt = 1e-9
  solve_type = LINEAR
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
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
