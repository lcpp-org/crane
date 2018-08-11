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

  # Spatial derivatives
  [./x_source]
    type = ReactantFirstOrder
    variable = x
    reaction = 'x->x+x'
    coefficient = 1
  [../]
  [./x_sink]
    type = ReactantSecondOrder
    variable = x
    v = y
    reaction = 'x+y->y'
    coefficient = 1
  [../]

  [./y_source]
    type = ReactantSecondOrder
    variable = y
    v = x
    reaction = 'y+x->y'
    coefficient = 1
  [../]
  [./y_sink]
    type = ReactantFirstOrder
    variable = y
    reaction = 'y->y+y'
    coefficient = 1
  [../]
[]

[Materials]
  [./rxn_x_source]
    type = GenericConstantMaterial
    prop_names = 'k_x->x+x'
    prop_values = '0.6666666666667'
  [../]
  [./rxn_x_sink]
    type = GenericConstantMaterial
    prop_names = 'k_x+y->y'
    prop_values = '-1.333333333333'
  [../]
  [./rxn_y_source]
    type = GenericConstantMaterial
    prop_names = 'k_y+x->y'
    prop_values = '1'
  [../]
  [./rxn_y_sink]
    type = GenericConstantMaterial
    prop_names = 'k_y->y+y'
    prop_values = '-1'
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  dt = 0.1
  num_steps = 500
  #solve_type = 'NEWTON'
  scheme = 'crank-nicolson'
[]

[Outputs]
  exodus = true
[]
