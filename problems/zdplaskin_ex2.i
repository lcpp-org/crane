[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 2
  elem_type = EDGE2
[]

[Variables]
  # Temperature variable
  [./Tgas]
    initial_condition = 300
  [../]

  # ODE variables
  [./e]
    initial_condition = 1e12
  [../]

  [./Ar+]
    initial_condition = 1e12
  [../]

  #[./Ar*]
  #  initial_condition = 1.0
  #[../]

  [./Ar]
    initial_condition = 3.219e24
  [../]
[]

[Kernels]
  # Time derivatives
  [./dT_dt]
    type = TimeDerivative
    variable = Tgas
  [../]
  [./de_dt]
    type = TimeDerivative
    variable = e
  [../]
  [./dAr_dt]
    type = TimeDerivative
    variable = Ar
  [../]
  [./dAr+_dt]
    type = TimeDerivative
    variable = Ar+
  [../]
  #[./dAr*_dt]
  #  type = TimeDerivative
  #  variable = Ar*
  #[../]
[]

# Initial density = 3.219e24 m^-3, based on formula:
# gas_density = 1.0e-6 * ( 101325.0d0 * gas_pressure / 760.0d0 ) / ( 1.38065d-23 * gas_temperature )
#
# Initial pressure: 100 Torr
# Note that CRANE uses units of meters, not centimeters.
[Materials]
  [./values]
    type = GenericConstantMaterial
    prop_names = 'gap_area resistance gap_length voltage'
    prop_values = '0.00005 1.0e5 0.004 1000'
  [../]
  [./masses]
    type = GenericConstantMaterial
    prop_names = 'masse massAr massAr+'
    prop_values = '9.11e-31 6.64e-26 6.64e-26'
  [../]
  #[./reduced_field]
  #  type = GenericConstantMaterial
  #  prop_names = 'reduced_field'
  #  prop_values = '1e-20'
  #[../]
  [./reduced_field]
    type = ElectricField
    electron_density = e
    neutral_density = Ar
    file_location = 'OutputRates_Crane'
  [../]
[]


[LotsOfTwoBodyReactions]
  species = 'e Ar Ar+'
  reaction_coefficient_format = 'rate'
  include_electrons = true
  electron_density = e
  file_location = 'OutputRates_Crane'
  reactions = 'e + Ar -> e + e + Ar+         : BOLOS'
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time = 1.0e-3
  dt = 1.0e-9
  #num_steps = 1
  solve_type = 'NEWTON'
  #scheme = 'crank-nicolson'
[]

[Outputs]
  exodus = true
[]
