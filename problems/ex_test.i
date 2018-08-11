[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 100
  elem_type = EDGE2
[]

[Variables]
  # ODE variables
  [./x1]
    initial_condition = 1
  [../]
[]

[NodalKernels]
  [./test]
    type = TimeDerivativeNodalKernel
    variable = x1
  [../]
  [./test1]
    type = ConstantRate
    variable = x1
    rate = 1.0
  [../]
[]

[Executioner]
  type = Transient
  start_time = 0
  dt = 0.1
  num_steps = 10

  #Preconditioned JFNK (default)
  solve_type = 'PJFNK'
[]

[Outputs]
  exodus = true
[]
