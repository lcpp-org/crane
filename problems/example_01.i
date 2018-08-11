[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
  elem_type = EDGE2
[]

[Variables]
  # ODE variables
  [./A]
    initial_condition = 100
  [../]
  [./B]
    initial_condition = 10
  [../]
[]

[NodalKernels]
  [./dA_dt]
    type = TimeDerivativeNodalKernel
    variable = A
  [../]
  [./dB_dt]
    type = TimeDerivativeNodalKernel
    variable = B
  [../]
  [./A_sink]
    type = ReactantARxn
    variable = A
    coefficient = -1.0
    rate_coefficient = 1.0
    #rate_coefficient = 2.5
  [../]
  [./A_source]
    type = ProductARxn
    variable = A
    v = B
    coefficient = 1.0
    rate_coefficient = 1.0
    #rate_coefficient = 0.5
  [../]
  [./B_sink]
    type = ReactantARxn
    variable = B
    coefficient = -1.0
    rate_coefficient = 1.0
    #rate_coefficient = 0.5
  [../]
  [./B_source]
    type = ProductARxn
    variable = B
    v = A
    coefficient = 1.0
    rate_coefficient = 1.0
    #rate_coefficient = 2.5
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time = 10
  trans_ss_check = true
  ss_check_tol = 1e-4
  [./TimeStepper]
    type = ConstantDT
    dt = 0.1
  [../]
  #Preconditioned JFNK (default)
  solve_type = 'JFNK'
[]

[Outputs]
  exodus = true
[]
