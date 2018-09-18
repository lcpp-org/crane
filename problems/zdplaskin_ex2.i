[GlobalParams]
  #use_log = true
[]

[Problem]
  type = FEProblem
  # type = EigenProblem
  # ignore_zeros_in_jacobian = true
  # use_nonlinear = false
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
  # elem_type = EDGE2
[]

[Variables]
  # Temperature variable
  #[./Tgas]
  #  initial_condition = 300
  #[../]

  # ODE variables
  [./e]
    initial_condition = 1e6
    # scaling = 1e-12
  [../]
  #
  # [./Ar+]
  #   initial_condition = 1e12
  # [../]

  # [./Ar*]
  #   initial_condition = 100
  #   #scaling = 1e-12
  # [../]

  #[./Ar]
  #  initial_condition = 3.2e24
  #  scaling = 1e-12
  #[../]

  # [./Ar2+]
  #   initial_condition = 100
  # [../]
[]

[Kernels]
  # Time derivatives
  [./de_dt]
    type = TimeDerivative
    variable = e
  [../]
  #[./dAr_dt]
  #  type = TimeDerivative
  #  variable = Ar
  #[../]
  # [./dAr+_dt]
  #   type = TimeDerivative
  #   variable = Ar+
  # [../]
  # [./dAr*_dt]
    # type = TimeDerivative
    # variable = Ar*
  # [../]
  # [./dAr2+_dt]
  #   type = TimeDerivative
  #   variable = Ar2+
  # [../]

  #
  # [./e_diff]
  #  type = ReactantFirstOrder
  #  reaction = 'e -> Ar'
  #  variable = e
  #  diffusion_term = true
  #  coefficient = -1
  # [../]
  # [./Ar+_diff]
  #  type = ReactantFirstOrder
  #  reaction = 'Ar+ -> Ar'
  #  variable = Ar+
  #  diffusion_term = true
  #  coefficient = -1
  # [../]
  # [./Ar2+_diff]
  #  type = ReactantFirstOrder
  #  reaction = 'Ar2+ -> Ar'
  #  variable = Ar2+
  #  diffusion_term = true
  #  coefficient = -1
  # [../]


  # Testing parsed material
  # [./rxn8_a]
  #  type = ReactantThirdOrder
  #  reaction = 'Ar+ + e + e -> Ar + e'
  #  variable = Ar+
  #  v = e
  #  w = e
  #  coefficient = -1
  # [../]
  #
  # [./rxn8_b]
  #  type = ReactantThirdOrder
  #  reaction = 'Ar+ + e + e -> Ar + e'
  #  variable = e
  #  v = e
  #  w = Ar+
  #  coefficient = -1
  #  v_eq_u = true
  # [../]



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
    prop_values = '0.004 0.00005 1.0e5 0.004 1000 3.2e24'
  [../]
  #[./masses]
  #  type = GenericConstantMaterial
  #  prop_names = 'masse massAr massAr+'
  #  prop_values = '9.11e-31 6.64e-26 6.64e-26'
  #[../]
  [./reduced_field]
   type = GenericConstantMaterial
   prop_names = 'reduced_field'
   #prop_values = '1e-20'
   prop_values = '7.76697e-20'
  [../]
  # [./reduced_field]
  #  type = ElectricField
  #  #use_log = true
  #  electron_density = e
  #  #neutral_density = Ar
  #  file_location = 'OutputRates_Crane'
  # [../]
  # [./diffusion_temp]
  #   type = DiffusionRateTemp
  #   file_location = 'OutputRates_Crane'
  # [../]
  #
  # [./eqn8]
  #   block = 0
  #   type = ParsedMaterial
  #   f_name = 'k_Ar+ + e + e -> Ar + e'
  #   constant_names = 'Te'
  #   constant_expressions = '50000' # in eV
  #   function = '8.75e-27 * (Te/11600.0)^(-4.5)'
  # [../]
[]


[ChemicalReactions]
  species = 'e'
  reaction_coefficient_format = 'rate'
  include_electrons = true
  electron_density = e
  file_location = 'OutputRates_Crane'

  reactions = 'e + Ar -> e + e + Ar+          : 1e-17'
               # e + Ar -> Ar* + e              : BOLOS'
               # e + Ar* -> Ar+ + e + e         : BOLOS'
               # Ar* + Ar* -> Ar2+ + e          : 6.0e-16'
               # e + Ar* -> Ar + e              : 1e-16
[]

[Executioner]
  # solver types:
  # implicit-euler explicit-euler crank-nicolson bdf2
  # explicit-midpoint dirk explicit-tvd-rk-2
  type = Transient
  # end_time = 1.0e-4
  dt = 1e-9
  # num_steps = 100
  solver = newton
  #petsc_options = '-snes_linesearch_monitor -pc_svd_monitor'
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
  # nl_max_its = 5
  dtmin = 1e-14
  # [./TimeStepper]
  #   type = IterationAdaptiveDT
  #   cutback_factor = 0.9
  #   dt = 1e-9
  #   growth_factor = 1.01
  #   optimal_iterations = 20
  # [../]
[]

[Preconditioning]
  active = 'smp'
  [./smp]
    type = SMP
    full = true
    #ksp_norm = none
  [../]

  [./fd]
    type = FDP
    full = true
    # petsc_options_iname = '-mat_fd_coloring_err -mat_fd_type'
    # petsc_options_value = '1e-6                 ds'
  [../]
[]

[Outputs]
  exodus = true
[]
