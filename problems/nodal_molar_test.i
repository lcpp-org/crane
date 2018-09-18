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
    initial_condition = -40.939
    #initial_condition = 13.8155
  [../]

  [./Ar+]
    initial_condition = -40.939
    #initial_condition = 13.8155
  [../]

  #[./Ar]
  #  initial_condition = 2.5e25
  #[../]
[]

[NodalKernels]
  [./de_dt]
    type = TimeDerivativeNodalKernelMolar
    variable = e
  [../]
  [./dAr+_dt]
    type = TimeDerivativeNodalKernelMolar
    variable = Ar+
  [../]
  #[./dAr_dt]
  #  type = TimeDerivativeNodalKernel
  #  variable = Ar
  #[../]


  #[./Ar_source]
  #  type = Product3Body
  #  variable = Ar
  #  v = e
  #  w = Ar+
  #  x = Ar
  #  _x_eq_u = true
  #  n_gas = 2.5e19
  #  rate_coefficient = 1.0e-25
  #  coefficient = 1
  #[../]
  [./e_source]
    type = Product2BodyMolar
    variable = e
    v = e
    #w = Ar
    #n_gas = 2.5e25
    n_gas = 41.5
    coefficient = 1
    #rate_coefficient = 7.25723e-18
    #rate_coefficient = 7.25723e-12
    rate_coefficient = 4.3703e6
    _v_eq_u = 1
  [../]
  [./Ar+_source]
    type = Product2BodyMolar
    variable = Ar+
    v = e
    #w = Ar
    #n_gas = 2.5e25
    n_gas = 41.5
    coefficient = 1
    #rate_coefficient = 7.25723e-18
    #rate_coefficient = 7.25723e-12
    rate_coefficient = 4.3703e6
  [../]

  #[./Ar_sink]
  #  type = Reactant2Body
  #  variable = Ar
  #  v = e
  #  coefficient = -1
  #  rate_coefficient = 7.25723e-12
  #[../]
  #[./e_sink]
  #  type = Reactant3Body
  #  variable = e
  #  v = Ar+
  #  w = Ar
  #  #n_gas = 2.5e25
  #  n_gas = 2.5e19
  #  coefficient = -1
  #  #rate_coefficient = 1.0e-37
  #  rate_coefficient = 1.0e-25
  #[../]
  #[./Ar+_sink]
  #  type = Reactant3Body
  #  variable = Ar+
  #  v = e
  #  w = Ar
  #  #n_gas = 2.5e25
  #  n_gas = 2.5e19
  #  coefficient = -1
  #  #rate_coefficient = 1.0e-37
  #  rate_coefficient = 1.0e-25
  #[../]
[]


#[ChemicalReactions]
#  species = 'e Ar+'
#  reaction_coefficient_format = 'rate'
#  include_electrons = true
#  electron_density = e
#  file_location = 'OutputRates_Crane'
#  reactions = 'e + Ar -> e + e + Ar+   : BOLOS'
#[]

[Debug]
  show_var_residual_norms = true
[]


[Executioner]
  # implicit-euler,  explicit-euler,  crank-nicolson,  bdf2,  explicit-midpoint,  dirk,  explicit-tvd-rk-2
  type = Transient
  end_time = 0.3e-5
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
