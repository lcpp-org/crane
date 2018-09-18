[GlobalParams]
  use_log = true
  offset = 40
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 4
  elem_type = EDGE2
[]

[Variables]
  # Temperature variable
  #[./Tgas]
  #  initial_condition = 300
  #[../]

  # ODE variables
  [./e]
    initial_condition = 27.63
    #initial_condition = -27.12
    #initial_condition = 1e12
    #scaling = 1e-12
  [../]

  [./Ar+]
    initial_condition = 27.63
    #initial_condition = -27.12
    #initial_condition = 1e12
    #scaling = 1e-12
  [../]

  [./Ar*]
    initial_condition = 4.605
    #initial_condition = -54
    #initial_condition = 1e12
    #scaling = 1e-12
  [../]

  #[./Ar]
  #  initial_condition = 3.2e24
  #  scaling = 1e-12
  #[../]
[]

[Kernels]
  [./de_dt]
    type = TimeDerivativeLog
    variable = e
  [../]
  [./dAr+_dt]
    type = TimeDerivativeLog
    variable = Ar+
  [../]
  [./dAr*_dt]
    type = TimeDerivativeLog
    variable = Ar*
  [../]

  #[./e_stable]
  #  type = LogStabilization
  #  variable = e
  #  #offset = 20
  #[../]
  #[./Ar+_stable]
  #  type = LogStabilization
  #  variable = Ar+
  #  #offset = 20
  #[../]
  #[./Ar*_stable]
  #  type = LogStabilization
  #  variable = Ar*
  #[../]

  ### Diffusion testing
  #[./e_diff]
  #  type = ReactantFirstOrderLog
  #  variable = e
  #  diffusion_term = true
  #  coefficient = -1
  #[../]
  #[./Ar+_diff]
  #  type = ReactantFirstOrderLog
  #  variable = Ar+
  #  diffusion_term = true
  #  coefficient = -1
  #[../]
[]

[AuxVariables]
  [./e_dens]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Ar+_dens]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Ar*_dens]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./e_dens]
    type = DensityLogConvert
    variable = e_dens
    density_log = e
    use_moles = false
  [../]
  [./Ar+_dens]
    type = DensityLogConvert
    variable = Ar+_dens
    density_log = Ar+
    use_moles = false
  [../]
  [./Ar*_dens]
    type = DensityLogConvert
    variable = Ar*_dens
    density_log = Ar*
    use_moles = false
  [../]
[]

# Initial density = 3.219e24 m^-3, based on formula:
# gas_density = 1.0e-6 * ( 101325.0d0 * gas_pressure / 760.0d0 ) / ( 1.38065d-23 * gas_temperature )
#
# Initial pressure: 100 Torr
# Note that CRANE uses units of meters, not centimeters.
[Materials]
  [./values]
    type = GenericConstantMaterial
    prop_names = 'radius gap_area resistance gap_length voltage n_gas'
    #prop_values = '0.004 0.00005 1.0e5 0.004 1000 5.345'
    prop_values = '0.004 0.00005 1.0e5 0.004 1000 3.219e24'
  [../]
  [./reduced_field]
    type = GenericConstantMaterial
    prop_names = 'reduced_field'
    #prop_values = '1e-20'
    prop_values = '7.76697e-20'
  [../]
  [./diffusion_temp]
    type = DiffusionRateTemp
    file_location = 'OutputRates_Crane'
  [../]
[]


[ChemicalReactions]
  species = 'e Ar+'
  reaction_coefficient_format = 'rate'
  include_electrons = true
  electron_density = e
  file_location = 'OutputRates_Crane'

  reactions = 'e + Ar -> e + e + Ar+          : BOLOS
               e + Ar -> Ar* + e              : BOLOS
               e + Ar* -> Ar+ + e + e         : BOLOS'
[]

[Executioner]
  type = Transient
  #line_search = none
  end_time = 1.0e-1
  #dt = 1.0e-7
  #num_steps = 100
  #petsc_options = '-snes_linesearch_monitor -pc_svd_monitor'
  #petsc_options_iname = '-pc_type'
  #petsc_options_value = 'svd'
  solve_type = jfnk
  l_max_its = 20
  dtmin = 1e-14
  #nl_rel_tol = 1e-4
  nl_abs_tol = 7.6e-5
  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.6
    dt = 1e-7
    growth_factor = 1.2
    optimal_iterations = 15
  [../]
[]

[Preconditioning]
  #[./smp]
  #  type = SMP
  #  full = true
  #  #ksp_norm = none
  #[../]

  #[./fd]
  #  type = FDP
  #  full = true
  #[../]
[]

[Outputs]
  exodus = true
[]
