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
    initial_condition = 1
  [../]

  [./y]
    initial_condition = 1
  [../]
[]

[Kernels]
  # Time derivatives
  [./dx_dt]
    type = TimeDerivative
    variable = x
  [../]
  [./dy_dt]
    type = TimeDerivative
    variable = y
  [../]
[]

[Materials]
  [./ngas]
    type = GenericConstantMaterial
    prop_names = 'n_gas'
    prop_values = 1
  [../]
[]

[ChemicalReactions]
  [./ZapdosNetwork]
  species = 'x y'
  reaction_coefficient_format = 'rate'
  scalar_problem = false
  reactions = 'x -> x + x             : 0.666667
               x + y -> y             : 1.333333
               y + x -> x + y + y     : 1
               y -> z                 : 1'
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time = 50
  dt = 0.1
  scheme = crank-nicolson
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Outputs]
  exodus = true
[]
