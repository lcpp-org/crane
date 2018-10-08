[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
[]

[Variables]
  [./x]
    order = FIRST
    family = SCALAR
    initial_condition = 1.0
  [../]

  [./y]
    order = FIRST
    family = SCALAR
    initial_condition = 1.0
  [../]
[]

[ScalarKernels]
  [./time1]
    type = ODETimeDerivative
    variable = x
  [../]

  [./time2]
    type = ODETimeDerivative
    variable = y
  [../]
[]


[ChemicalReactions]
  [./ScalarNetwork]
    species = 'x y'

    reactions = 'x -> x + x     : {2/3}
                 x + y -> y     : {4/3}
                 y -> z         : 1
                 y + x -> x + y + y : 1'
  [../]
[]

[Executioner]
  type = Transient
  end_time = 50
  dt = 0.1
  scheme = crank-nicolson
[]

[Outputs]
  csv = true
[]
