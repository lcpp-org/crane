[GlobalParams]
  #use_log = true
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 2
  elem_type = EDGE2
[]

[Variables]
  # ODE variables
  [./e]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]

  [./Ar+]
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
  [./dAr+_dt]
    type = ODETimeDerivative
    variable = Ar+
  [../]

  [./e_source]
    type = Product2BodyScalar
    variable = e
    v = e
    v_eq_u = true
    n_gas = 2.5e19
    coefficient = 1
    reaction_coefficient = 7.25723e-12
  [../]
  [./Ar+_source]
    type = Product2BodyScalar
    variable = Ar+
    v = e
    n_gas = 2.5e19
    coefficient = 1
    reaction_coefficient = 7.25723e-12
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]


[Executioner]
  # implicit-euler,  explicit-euler,  crank-nicolson,  bdf2,  explicit-midpoint,  dirk,  explicit-tvd-rk-2
  type = Transient
  end_time = 0.3e-6
  dt = 0.5e-8
  #num_steps = 5
  #solve_type = PJFNK
  #solve_type = NEWTON
  solve_type = 'JFNK'
  #scheme = 'implicit-euler'
  l_max_its = 20
  dtmin = 1e-14
[]

[Outputs]
  exodus = true
[]
