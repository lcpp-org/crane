[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 10
  elem_type = EDGE3
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

[NodalKernels]
  # Time derivatives
  [./dx_dt]
    type = TimeDerivativeNodalKernel
    variable = x
  [../]
  [./dy_dt]
    type = TimeDerivativeNodalKernel
    variable = y
  [../]

  # Spatial derivatives
  [./x_source]
    type = ReactantARxn
    variable = x
    coefficient = 1
    rate_coefficient = 0.66667
  [../]
  [./x_sink]
    type = Reactant2Body
    variable = x
    v = y
    coefficient = 1
    rate_coefficient = -1.33333
  [../]

  [./y_source]
    type = Reactant2Body
    variable = y
    v = x
    coefficient = 1
    rate_coefficient = 1
  [../]
  [./y_sink]
    type = ReactantARxn
    variable = y
    coefficient = 1
    rate_coefficient = -1
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  dt = 0.1
  num_steps = 500
  solve_type = 'NEWTON'
[]

[Outputs]
  exodus = true
[]
