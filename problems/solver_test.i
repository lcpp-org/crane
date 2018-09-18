[GlobalParams]
  #use_log = true
[]

[Problem]
  type = FEProblem
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
[]

[Kernels]
  # Time derivatives
  [./de_dt]
    type = TimeDerivative
    variable = e
  [../]
[]

[Materials]
  [./values]
    type = GenericConstantMaterial
    prop_names = 'n_gas'
    prop_values = '1'
  [../]
  [./reduced_field]
   type = GenericConstantMaterial
   prop_names = 'reduced_field'
   prop_values = '7.76697e-20'
  [../]
[]


[ChemicalReactions]
  species = 'e'
  reaction_coefficient_format = 'rate'
  include_electrons = true
  electron_density = e
  file_location = 'OutputRates_Crane'

  reactions = 'e + Ar -> e + e + Ar+          : 1'
[]

[Executioner]
  # solver types:
  # implicit-euler, explicit-euler, crank-nicolson,
  # bdf2, explicit-midpoint, dirk, explicit-tvd-rk-2
  type = Transient
  # end_time = 1.0e-4
  dt = 1.1
  num_steps = 1
  solve_type = newton
  nl_max_its = 5
  dtmin = 1e-7
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
  [../]
[]

[Outputs]
  exodus = true
[]
