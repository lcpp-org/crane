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
    initial_condition = 1e6
    #scaling = 1e-12
    #scaling = 1e-6
  [../]

  [./Ar+]
    initial_condition = 1e6
    #scaling = 1e-12
    #scaling = 1e-6
  [../]

  #[./Ar]
  #  initial_condition = 2.5e25
  #  #scaling = 2.5e-19
  #  scaling = 2.5e-25
  #[../]
[]

[Kernels]
  [./de_dt]
    type = TimeDerivative
    variable = e
  [../]
  [./dAr+_dt]
    type = TimeDerivative
    variable = Ar+
  [../]
  #[./dAr_dt]
  #  type = TimeDerivative
  #  variable = Ar
  #[../]

  [./e_source]
    type = ProductSecondOrder
    variable = e
    v = e
    coefficient = 1
    reaction = 'e + Ar -> e + e + Ar+'
    _v_eq_u = true
  [../]
#  [./Ar+_source]
#    type = ProductSecondOrder
#    variable = Ar+
#    v = e
#    coefficient = 1
#    reaction = 'e + Ar -> e + e + Ar+'
#    _v_eq_u = false
#  [../]
[]

[Materials]
  [./reduced_field]
    type = GenericConstantMaterial
    prop_names = 'reduced_field n_gas'
    prop_values = '5.0e-20 2.5e25'
  [../]
  [./rxn_coeff]
    type = EEDFRateConstant
    file_location = ''
    property_file = 'OutputRates_Crane/reaction_e + Ar -> e + e + Ar+.txt'
    position_units = 1
    sampling_format = 'reduced_field'
    reaction = 'e + Ar -> e + e + Ar+'
  [../]
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

[Preconditioning]
  #[./smp]
  #  type = SMP
  #  full = true
  #[../]

  # Finite-difference jacobian
  #[./fdp]
  #  type = FDP
  #  full = true
  #[../]
[]

#[Executioner]
#  type = Transient
#  end_time = 0.25e-6
#  petsc_options = '-snes_converged_reason -snes_linesearch_monitor'
#  #solve_type = PJFNK
#  solve_type = NEWTON
#  petsc_options_iname = '-pc_type -pc_factor_shift_type -pc_factor_shift_amount -ksp_type -snes_linesearch_minlambda'
#  petsc_options_value = 'lu NONZERO 1.e-10 preonly 1e-3'
#  nl_rel_tol = 1e-4
#  nl_abs_tol = 7.6e-5
#  dtmin = 1e-14
#  l_max_its = 20
#  #[./TimeStepper]
#  #  type = IterationAdaptiveDT
#  #  cutback_factor = 0.4
#  #  dt = 1e-11
#  #  growth_factor = 1.2
#  #  optimal_iterations = 20
#  #[../]
#[]


[Executioner]
  type = Transient
  end_time = 0.25e-6
  dt = 0.5e-9
  #num_steps = 5
  #solve_type = PJFNK
  solve_type = NEWTON
  #solve_type = 'JFNK'
  petsc_options = '-snes_linesearch_monitor -pc_svd_monitor'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'svd'
  #scheme = 'crank-nicolson'
  l_max_its = 20
  dtmin = 1e-14
[]

[Outputs]
  exodus = true
[]
