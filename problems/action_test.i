[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 10
  elem_type = EDGE2
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


[LotsOfTwoBodyReactions]
  species = 'x y'
  reaction_coefficient_format = 'rate'
  reactions = 'x -> x + x             : 0.666667
               x + y <-> y             : 1.333333
               y + x -> x + y + y     : 1
               y -> z                 : 1
               z -> z                 : {T * exp(Tgas/300)}'
[]

[Executioner]
  type = Transient
  start_time = 0
  dt = 0.1
  num_steps = 1
  #solve_type = 'NEWTON'
  scheme = 'crank-nicolson'
[]

[Outputs]
  exodus = true
[]
