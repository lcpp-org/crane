#dom0Scale=25.4e-3
dom0Scale=1.0

[GlobalParams]
  potential_units = kV
  use_moles = true
  #offset = 28
  offset = 20
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
[]

[Problem]
  type = FEProblem
[]

[Variables]
  [./e]
    initial_condition = 1e10
  [../]

  [./N2v1]
    initial_condition = 1
  [../]

  [./N2v2]
    initial_condition = 
  [../]

  [./N2v3]
    initial_condition = 
  [../]

  [./N2v4]
    initial_condition = 
  [../]

  [./N2v5]
    initial_condition = 
  [../]

  [./N2v6]
    initial_condition = 
  [../]

  [./N2v7]
    initial_condition = 
  [../]

  [./N2v8]
    initial_condition = 
  [../]

  [./N2A3]
    initial_condition = 
  [../]

  [./N2B3]
    initial_condition = 
  [../]

  [./N2a_1]
    initial_condition = 
  [../]

  [./N2C3]
    initial_condition = 
  [../]

  [./N]
    initial_condition = 
  [../]

  [./N2D]
    initial_condition = 
  [../]

  [./N2P]
    initial_condition = 
  [../]

  [./N+]
    initial_condition = 
  [../]

  [./N2+]
    initial_condition = 
  [../]

  [./N3+]
    initial_condition = 
  [../]

  [./N4+]
    initial_condition = 
  [../]

  [./O2]
    initial_condition = 
  [../]

  [./O2v1]
    initial_condition = 
  [../]

  [./O2v2]
    initial_condition = 
  [../]

  [./O2v3]
    initial_condition = 
  [../]

  [./O2v4]
    initial_condition = 
  [../]

  [./O2a1]
    initial_condition = 
  [../]

  [./O2b1]
    initial_condition = 
  [../]

  [./O24_ev]
    initial_condition = 
  [../]

  [./O]
    initial_condition = 
  [../]

  [./O1D]
    initial_condition = 
  [../]

  [./O1S]
    initial_condition = 
  [../]

  [./O3]
    initial_condition = 
  [../]

  [./O+]
    initial_condition = 
  [../]

  [./O2+]
    initial_condition = 
  [../]

  [./O4+]
    initial_condition = 
  [../]

  [./O-]
    initial_condition = 
  [../]

  [./O2-]
    initial_condition = 
  [../]

  [./O3-]
    initial_condition = 
  [../]

  [./O4-]
    initial_condition = 
  [../]

  [./NO]
    initial_condition = 
  [../]

  [./N2O]
    initial_condition = 
  [../]

  [./NO2]
    initial_condition = 
  [../]

  [./NO3]
    initial_condition = 
  [../]

  [./N2O5]
    initial_condition = 
  [../]

  [./NO+]
    initial_condition = 
  [../]

  [./N2O+]
    initial_condition = 
  [../]

  [./NO2+]
    initial_condition = 
  [../]

  [./NO-]
    initial_condition = 
  [../]

  [./N2O-]
    initial_condition = 
  [../]

  [./NO2-]
    initial_condition = 
  [../]

  [./NO3-]
    initial_condition = 
  [../]

  [./OHpN2]
    initial_condition = 
  [../]

[]
[Kernels]
  [./em_log_stabilization]
    type = LogStabilizationMoles
    variable = em
    block = 0
  [../]
  [./Arp_log_stabilization]
    type = LogStabilizationMoles
    variable = Ar+
    block = 0
  [../]
  [./mean_en_log_stabilization]
    type = LogStabilizationMoles
    variable = mean_en
    block = 0
    offset = 15
  [../]

  [./em_time_deriv]
    type = ElectronTimeDerivative
    variable = em
    block = 0
  [../]
  [./em_advection]
    type = EFieldAdvectionElectrons
    variable = em
    potential = potential
    mean_en = mean_en
    block = 0
    position_units = ${dom0Scale}
  [../]
  [./em_diffusion]
    type = CoeffDiffusionElectrons
    variable = em
    mean_en = mean_en
    block = 0
    position_units = ${dom0Scale}
  [../]

  [./potential_diffusion_dom1]
    type = CoeffDiffusionLin
    variable = potential
    block = 0
    position_units = ${dom0Scale}
  [../]
  [./Arp_charge_source]
    type = ChargeSourceMoles_KV
    variable = potential
    charged = Ar+
    block = 0
  [../]
  [./em_charge_source]
    type = ChargeSourceMoles_KV
    variable = potential
    charged = em
    block = 0
  [../]

  [./Arp_time_deriv]
    type = ElectronTimeDerivative
    variable = Ar+
    block = 0
  [../]
  [./Arp_advection]
    type = EFieldAdvection
    variable = Ar+
    potential = potential
    position_units = ${dom0Scale}
    block = 0
  [../]
  [./Arp_diffusion]
    type = CoeffDiffusion
    variable = Ar+
    block = 0
    position_units = ${dom0Scale}
  [../]

  [./mean_en_time_deriv]
    type = ElectronTimeDerivative
    variable = mean_en
    block = 0
  [../]
  [./mean_en_advection]
    type = EFieldAdvectionEnergy
    variable = mean_en
    potential = potential
    em = em
    block = 0
    position_units = ${dom0Scale}
  [../]
  [./mean_en_diffusion]
    type = CoeffDiffusionEnergy
    variable = mean_en
    em = em
    block = 0
    position_units = ${dom0Scale}
  [../]
  [./mean_en_joule_heating]
    type = JouleHeating
    variable = mean_en
    potential = potential
    em = em
    block = 0
    position_units = ${dom0Scale}
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]

[AuxVariables]
  [./Te]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./x]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./x_node]
  [../]

  [./rho]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./em_lin]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Ar+_lin]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Ar]
  [../]

  [./Efield]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./Current_em]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
  [./Current_Ar]
    order = CONSTANT
    family = MONOMIAL
    block = 0
  [../]
[]

[AuxKernels]
  [./Te]
    type = ElectronTemperature
    variable = Te
    electron_density = em
    mean_en = mean_en
    execute_on = 'initial linear nonlinear'
  [../]
  [./x_g]
    type = Position
    variable = x
    position_units = ${dom0Scale}
  [../]

  [./x_ng]
    type = Position
    variable = x_node
    position_units = ${dom0Scale}
  [../]

  [./em_lin]
    type = DensityMoles
    convert_moles = true
    variable = em_lin
    density_log = em
  [../]
  [./Ar+_lin]
    type = DensityMoles
    convert_moles = true
    variable = Ar+_lin
    density_log = Ar+
  [../]

  [./Ar_val]
    type = ConstantAux
    variable = Ar
    # value = 3.22e22
    #value = -2.928623
    value = 3.704333
    execute_on = INITIAL
  [../]

  [./Efield_calc]
    type = Efield
    component = 0
    potential = potential
    variable = Efield
    position_units = ${dom0Scale}
  [../]
  [./Current_em]
    type = Current
    potential = potential
    density_log = em
    variable = Current_em
    art_diff = false
    block = 0
    position_units = ${dom0Scale}
  [../]
  [./Current_Ar]
    type = Current
    potential = potential
    density_log = Ar+
    variable = Current_Ar
    art_diff = false
    block = 0
    position_units = ${dom0Scale}
  [../]
[]

[UserObjects]
  [./data_provider]
    type = ProvideMobility
    electrode_area = 5.02e-7 # Formerly 3.14e-6
    ballast_resist = 1e6
    e = 1.6e-19
    # electrode_area = 1.1
    # ballast_resist = 1.1
    # e = 1.1
  [../]
[]

[BCs]
  [./potential_left]
    type = NeumannCircuitVoltageMoles_KV
    variable = potential
    boundary = left
    function = potential_bc_func
    ip = Ar+
    data_provider = data_provider
    em = em
    mean_en = mean_en
    r = 0
    position_units = ${dom0Scale}
  [../]
  [./potential_dirichlet_right]
    type = DirichletBC
    variable = potential
    boundary = right
    value = 0
  [../]
  [./em_physical_right]
    type = HagelaarElectronBC
    variable = em
    boundary = 'right'
    potential = potential
    ip = Ar+
    mean_en = mean_en
    #r = 0.99
    r = 0.0
    position_units = ${dom0Scale}
  [../]
  # [./em_physical_right]
  #   type = MatchedValueLogBC
  #   variable = em
  #   boundary = 'master0_interface'
  #   v = emliq
  #   H = 1e3
  # [../]
  [./Arp_physical_right_diffusion]
    type = HagelaarIonDiffusionBC
    variable = Ar+
    boundary = 'right'
    r = 0
    position_units = ${dom0Scale}
  [../]
  [./Arp_physical_right_advection]
    type = HagelaarIonAdvectionBC
    variable = Ar+
    boundary = 'right'
    potential = potential
    r = 0
    position_units = ${dom0Scale}
  [../]
  [./mean_en_physical_right]
    type = HagelaarEnergyBC
    variable = mean_en
    boundary = 'right'
    potential = potential
    em = em
    ip = 'Ar+'
    #r = 0.99
    r = 0.0
    position_units = ${dom0Scale}
  [../]
  [./em_physical_left]
    type = HagelaarElectronBC
    variable = em
    boundary = 'left'
    potential = potential
    ip = Ar+
    mean_en = mean_en
    r = 0
    position_units = ${dom0Scale}
  [../]
  #[./sec_electrons_left]
  #  type = SecondaryElectronBC
  #  variable = em
  #  boundary = 'left'
  #  potential = potential
  #  ip = Arp
  #  mean_en = mean_en
  #  r = 0
  #  position_units = ${dom0Scale}
  #[../]
  [./Arp_physical_left_diffusion]
    type = HagelaarIonDiffusionBC
    variable = Ar+
    boundary = 'left'
    r = 0
    position_units = ${dom0Scale}
  [../]
  [./Arp_physical_left_advection]
    type = HagelaarIonAdvectionBC
    variable = Ar+
    boundary = 'left'
    potential = potential
    r = 0
    position_units = ${dom0Scale}
  [../]
  [./mean_en_physical_left]
    type = HagelaarEnergyBC
    variable = mean_en
    boundary = 'left'
    potential = potential
    em = em
    ip = Ar+
    r = 0
    position_units = ${dom0Scale}
  [../]
[]

[ICs]
  [./em_ic]
    type = ConstantIC
    variable = em
    value = -21
    block = 0
  [../]
  [./Arp_ic]
    type = ConstantIC
    variable = Ar+
    value = -21
    block = 0
  [../]
  [./mean_en_ic]
    type = ConstantIC
    variable = mean_en
    value = -20
    block = 0
  [../]
  [./potential_ic]
    type = FunctionIC
    variable = potential
    function = potential_ic_func
  [../]
[]

[Functions]
  [./potential_bc_func]
    type = ParsedFunction
    #value = '0.100*sin(2*3.1415926*13.56e6*t)'
    #value = '-10*tanh(1e9*t) - 0.1'
    #value = '1.25'
    #vars = 'sig mu'
    #vals = '1e-19 1e-9'
    ##value = '1.0/((2*3.1415926*sig)^0.5) * exp(-((t-mu)^2)/(2*sig))'
    #value = 'exp(-((t-mu)^2)/(2*sig))'
    value = 0.3
  [../]
  [./potential_ic_func]
    type = ParsedFunction
    #value = '-1.25 * (1.0e-2 - x)'
    #value = '-0.8 * (1.0e-2 - x)'
    value = '-0.3 * (1.0001e-3 - x)'
  [../]
  #[./density_ic_func]
  #  type = ParsedFunction
  #  value = 'log((1e13 + 1e15 * (1-x/1)^2 * (x/1)^2)/6.022e23)'
  #[../]
  #[./energy_density_ic_func]
  #  type = ParsedFunction
  #  value = 'log(3./2.) + log((1e13 + 1e15 * (1-x/1)^2 * (x/1)^2)/6.022e23)'
  #[../]
[]

[Materials]
  [./GasBasics]
    type = GasElectronMoments
    interp_trans_coeffs = true
    interp_elastic_coeff = true
    ramp_trans_coeffs = false
    #user_p_gas = 133.322
    #user_p_gas = 101325
    user_p_gas = 1.01e5
    em = em
    potential = potential
    mean_en = mean_en
    user_se_coeff = 0.05
    #user_electron_mobility = 30.0
    #user_electron_diffusion_coeff = 119.8757763975
    #property_tables_file = Argon_reactions_paper_RateCoefficients/electron_moments.txt
    property_tables_file = 'dc_argon_only/moments.txt'
    position_units = ${dom0Scale}
  [../]
  [./gas_species_0]
    type = HeavySpeciesMaterial
    heavy_species_name = Ar+
    heavy_species_mass = 6.64e-26
    heavy_species_charge = 1.0
    #mobility = 0.144409938
    #diffusivity = 6.428571e-3
  [../]
  #[./gas_block]
  #  type = Gas
  #  interp_trans_coeffs = true
  #  interp_elastic_coeff = true
  #  ramp_trans_coeffs = false
  #  em = em
  #  potential = potential
  #  ip = Arp
  #  mean_en = mean_en
  #  user_se_coeff = .05
  #  block = 0
  #  property_tables_file = td_argon_mean_en.txt
 #[../]
  [./gas_species_2]
    type = HeavySpeciesMaterial
    heavy_species_name = Ar
    heavy_species_mass = 6.64e-26
    heavy_species_charge = 0.0
  [../]
[]

[Preconditioning]
  active = 'smp'
  [./smp]
    type = SMP
    full = true
  [../]

  [./fdp]
    type = FDP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  automatic_scaling = true
  compute_scaling_once = false
  end_time = 1e-1
  line_search = 'basic'
  # end_time = 10
  petsc_options = '-snes_converged_reason -snes_linesearch_monitor'
  # petsc_options = '-snes_test_display'
  solve_type = NEWTON
  #solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_factor_shift_type -pc_factor_shift_amount -snes_linesearch_minlambda'
  petsc_options_value = 'lu NONZERO 1.e-10 1e-3'
  # petsc_options_iname = '-pc_type -sub_pc_type'
  # petsc_options_value = 'asm lu'
  # petsc_options_iname = '-snes_type'
  # petsc_options_value = 'test'
  nl_rel_tol = 1e-8
  #nl_abs_tol = 1e-10
  dtmin = 1e-12
  l_max_its = 20
  #num_steps = 54
  steady_state_detection = true
  [./TimeStepper]
    type = IterationAdaptiveDT
    cutback_factor = 0.4
    dt = 1e-11
    # dt = 1.1
    growth_factor = 1.2
   optimal_iterations = 15
  [../]
[]

[Outputs]
  print_perf_log = true
  [./out]
    type = Exodus
  [../]
[]

[Reactions]
  [./Air]
    species = 'N2v1 N2v2 N2v3 N2v4 N2v5 N2v6 N2v7 N2v8 N2A3 N2B3 N2a_1 N2C3 N N2D N2P N+ N2+ N3+ N4+ O2 O2v1 O2v2 O2v3 O2v4 O2a1 O2b1 O2_4ev O O1D O1S O3 O+ O2+ O4+ O- O2- O3- O4- NO N2O NO2 NO3 N2O5 NO+ N2O+ NO2+ NO- N2O- NO2- NO3- O2pN2 e'
    aux_species = 'N2'
    reaction_coefficient_format = 'townsend'
    gas_species = 'N2'
    electron_energy = 'mean_en'
    electron_density = 'em'
    include_electrons = true
    #file_location = 'Argon_reactions_paper_RateCoefficients'
    file_location = 'air_test'
    #equation_variables = 'mean_en em'
    equation_variables = 'Te Tgas'
    potential = 'potential'
    use_log = true
    position_units = ${dom0Scale}
    block = 0


    # NOTE: rate coefficients units are cm^3 s^-1
    reactions = 'e + N2 -> e + N2v1                 : EEDF
                 e + N2 -> e + N2v1                 : EEDF (N2v1res) 
                 e + N2 -> e + N2v2                 : EEDF 
                 e + N2 -> e + N2v3                 : EEDF 
                 e + N2 -> e + N2v4                 : EEDF 
                 e + N2 -> e + N2v5                 : EEDF 
                 e + N2 -> e + N2v6                 : EEDF 
                 e + N2 -> e + N2v7                 : EEDF 
                 e + N2 -> e + N2v8                 : EEDF 
                 e + N2v1 -> e + N2                 : EEDF
                 e + N2v2 -> e + N2                 : EEDF
                 e + N2v3 -> e + N2                 : EEDF
                 e + N2v4 -> e + N2                 : EEDF
                 e + N2v5 -> e + N2                 : EEDF
                 e + N2v6 -> e + N2                 : EEDF
                 e + N2v7 -> e + N2                 : EEDF
                 e + N2v8 -> e + N2                 : EEDF
                 e + O2 -> e + O2v1                 : EEDF
                 e + O2 -> e + O2v1                 : EEDF (O2v1res)
                 e + O2 -> e + O2v2                 : EEDF
                 e + O2 -> e + O2v2                 : EEDF (O2v2res)
                 e + O2 -> e + O2v3                 : EEDF
                 e + O2 -> e + O2v4                 : EEDF
                 e + O2v1 -> e + O2                 : EEDF
                 e + O2v2 -> e + O2                 : EEDF
                 e + O2v3 -> e + O2                 : EEDF
                 e + O2v4 -> e + O2                 : EEDF
                 ######
                 # Vibrational-translational relaxation (Capitelli2000, page 105)
                 ######
                 N2v1 + N2 -> N2 + N2               : {7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v2 + N2 -> N2v1 + N2             : {2.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v3 + N2 -> N2v2 + N2             : {3.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v4 + N2 -> N2v3 + N2             : {4.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v5 + N2 -> N2v4 + N2             : {5.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v6 + N2 -> N2v5 + N2             : {6.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v7 + N2 -> N2v6 + N2             : {7.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2v8 + N2 -> N2v7 + N2             : {8.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas))}
                 N2 + N2 -> N2v1 + N2               : {7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v1 + N2 -> N2v2 + N2             : {2.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 Nvv2 + N2 -> N2v3 + N2             : {3.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v3 + N2 -> N2v4 + N2             : {4.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v4 + N2 -> N2v5 + N2             : {5.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v5 + N2 -> N2v6 + N2             : {6.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v6 + N2 -> N2v7 + N2             : {7.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v7 + N2 -> N2v8 + N2             : {8.0 * 7.8e-12 * Tgas * exp(-218.0 / Tgas^(1./3.) + 690. / Tgas) / (1.0 - exp(-0.290*11605/Tgas)) * exp(-0.29*11605/Tgas)}
                 N2v1 + N -> N2 + N                 : {4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v2 + N -> N2v1 + N               : {2.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v3 + N -> N2v2 + N               : {3.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v4 + N -> N2v3 + N               : {4.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v5 + N -> N2v4 + N               : {5.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v6 + N -> N2v5 + N               : {6.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v7 + N -> N2v6 + N               : {7.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2v8 + N -> N2v7 + N               : {8.0 * 4.0e-16 * (Tgas / 300.0)^0.5}
                 N2 + N -> N2v1 + N                 : {4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v1 + N -> N2v2 + N               : {2.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 Nvv2 + N -> N2v3 + N               : {3.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v3 + N -> N2v4 + N               : {4.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v4 + N -> N2v5 + N               : {5.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v5 + N -> N2v6 + N               : {6.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v6 + N -> N2v7 + N               : {7.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v7 + N -> N2v8 + N               : {8.0 * 4.0e-16 * (Tgas / 300.0)^0.5 * exp(-0.29*11605.0)}
                 N2v1 + O -> N2 + O                 : {1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v2 + O -> N2v1 + O               : {2.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v3 + O -> N2v2 + O               : {3.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v4 + O -> N2v3 + O               : {4.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v5 + O -> N2v4 + O               : {5.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v6 + O -> N2v5 + O               : {6.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v7 + O -> N2v6 + O               : {7.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2v8 + O -> N2v7 + O               : {8.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0))}
                 N2 + O -> N2v1 + O                 : {1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v1 + O -> N2v2 + O               : {2.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v2 + O -> N2v3 + O               : {3.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v3 + O -> N2v4 + O               : {4.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v4 + O -> N2v5 + O               : {5.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v5 + O -> N2v6 + O               : {6.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v6 + O -> N2v7 + O               : {7.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 N2v7 + O -> N2v8 + O               : {8.0 * 1.20e-13 * exp( - 27.6 / Tgas^(1.0/3.0)) * exp(-0.28*11605.0)}
                 O2v1 + O2 -> O2 + O2               : {1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) )}
                 O2v2 + O2 -> O2v1 + O2             : {2.0 * 1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) )}
                 O2v3 + O2 -> O2v2 + O2             : {3.0 * 1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) )}
                 O2v4 + O2 -> O2v3 + O2             : {4.0 * 1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) )}
                 O2 + O2 -> O2v1 + O2               : {1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) ) * exp(-0.19 * 11605)}
                 O2v1 + O2 -> O2v2 + O2             : {2.0 * 1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) ) * exp(-0.19 * 11605)}
                 O2v2 + O2 -> O2v3 + O2             : {3.0 * 1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) ) * exp(-0.19 * 11605)}
                 O2v3 + O2 -> O2v4 + O2             : {4.0 * 1.35e-12 * Tgas * exp( - 137.9 / Tgas^(1.0/3.0) ) / ( 1.0 - exp(-0.19*11605.0) ) * exp(-0.19 * 11605)}
                 O2v1 + O -> O2 + O                 : {4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2v2 + O -> O2v1 + O               : {2.0 * 4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2v3 + O -> O2v2 + O               : {3.0 * 4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2v4 + O -> O2v3 + O               : {4.0 * 4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2 + O -> O2v1 + O                 : {4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2v1 + O -> O2v2 + O               : {2.0 * 4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2v2 + O -> O2v3 + O               : {3.0 * 4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 O2v3 + O -> O2v4 + O               : {4.0 * 4.5e-15 * Tgas * exp(-0.19 * 11605)}
                 #####
                 # excitation of electronic levels by electron impact (Bolsig+)
                 # Note that CRANE will need to be modified to allow duplicate reactions here...
                 #####
                 e + N2 -> e + N2A3                 : EEDF (N2 -> N2A3)
                 e + N2 -> e + N2A3                 : EEDF (N2 -> N2A3,v5-9)
                 e + N2 -> e + N2A3                 : EEDF (N2 -> N2A3,v10-)
                 e + N2 -> e + N2B3                 : EEDF (N2 -> N2(B3))
                 e + N2 -> e + N2B3                 : EEDF (N2 -> N2(W3))
                 e + N2 -> e + N2B3                 : EEDF (N2 -> N2(B'3))
                 e + N2 -> e + N2a_1                : EEDF (N2 -> N2(a'1))
                 e + N2 -> e + N2a_1                : EEDF (N2 -> N2(a1))
                 e + N2 -> e + N2a_1                : EEDF (N2 -> N2(w1))
                 e + N2 -> e + N2C3                 : EEDF (N2 -> N2(C3))
                 e + N2 -> e + N2C3                 : EEDF (N2 -> N2(E3))
                 e + N2 -> e + N2C3                 : EEDF (N2 -> N2(a''1))
                 e + N2 -> e + N + N2D              : EEDF (N2 -> N2(SUM))
                 e + O2 -> e + O2a1                 : EEDF (O2 -> O2(a1))
                 e + O2 -> e + O2b1                 : EEDF (O2 -> O2(b1))
                 e + O2 -> e + O2_4ev               : EEDF (O2 -> O2(4.5ev))
                 e + O2 -> e + O + O                : EEDF (O2 -> O2(6.0ev))
                 e + O2 -> e + O + O1D              : EEDF (O2 -> O2(8.4ev))
                 e + O2 -> e + O + O1S              : EEDF (O2 -> O2(9.97ev))
                 e + O2a1 -> e + O + O             : EEDF (O2(a1) -> O + O)
                 e + O -> e + O1D                   : EEDF (O -> O(1D))
                 e + O -> e + O1S                   : EEDF (O -> O(1S))
                 ####
                 # de-excitation of electronic levels by electron impact (BOLSIG+)
                 ####
                 e + N2A3 -> e + N2                 : EEDF (N2(A3) -> N2)
                 e + O2a1 -> e + O2                : EEDF (O2(a1) -> O2)
                 ####
                 # ionization by electron impact (BOLSIG+)
                 # note the missing data section in the inp file (4 extra reactions - not shown here)
                 ####
                 e + N -> e + e + N+                : EEDF
                 e + O -> e + e + O+                : EEDF
                 e + N2 -> e + e + N2+              : EEDF
                 e + N2A3 -> e + e + N2+            : EEDF
                 e + O2 -> e + e + O2+              : EEDF
                 e + O2a1 -> e + e + O2+            : EEDF
                 e + NO -> e + e + NO+              : EEDF
                 e + N2O -> e + e + N2O+            : EEDF
                 ####
                 # electron-ion recombination
                 ####
                 e + N2+ -> N + N                   : {1.8e-7 * (300/Te)^0.39 * 0.5}
                 e + N2+ -> N + N2D                 : {1.8e-7 * (300/Te)^0.39 * 0.45}
                 e + N2+ -> N + N2P                 : {1.8e-7 * (300/Te)^0.39 * 0.05}
                 e + O2+ -> O + O                   : {2.7e-7 * (300/Te)^0.7 * 0.55}
                 e + O2+ -> O + O1D                 : {2.7e-7 * (300/Te)^0.7 * 0.40}
                 e + O2+ -> O + O1S                 : {2.7e-7 * (300/Te)^0.7 * 0.05}
                 e + NO+ -> O + N                   : {4.2e-7 * (300/Te)^0.85 * 0.2}
                 e + NO+ -> O + N2D                 : {4.2e-7 * (300/Te)^0.85 * 0.8}
                 e + N3+ -> N2 + N                  : {2.0e-7 * (300/Te)^0.5}
                 e + N4+ -> N2 + N                  : {2.3e-6 * (300/Te)^0.53}
                 e + N2O+ -> N2 + O                 : {2.0e-7 * (300/Te)^0.5}
                 e + NO2+ -> NO + O                 : {2.0e-7 * (300/Te)^0.5}
                 e + O4+ -> O2 + O2                 : {1.4e-6 * (300/Te)^0.5}
                 e + O2pN2 -> O2 + N2               : {1.3e-6 * (300/Te)^0.5}
                 e + N+ -> N + e                    : {7.0e-20 * (300/Te)^4.5}
                 e + O+ -> O + e                    : {7.0e-20 * (300/Te)^4.5}
                 e + N+ + NEUTRAL -> N + NEUTRAL    : {6.0e-27 * (300/Te)^1.5}
                 e + O+ + NEUTRAL -> O + NEUTRAL    : {6.0e-27 * (300/Te)^1.5}
                 ####
                 # electron attachment
                 ####
                 e + O2 -> O- + O                     : EEDF
                 e + NO -> O- + N                     : EEDF
                 e + O3 -> O- + O2                    : EEDF
                 e + O3 -> O2- + O                    : EEDF
                 e + N2O -> NO- + N                   : EEDF
                 e + O2 + O2 -> O2- + O2              : EEDF
                 e + NO2 -> O- + NO                   : 1e-11
                 e + O + O2 -> O- + O2                : 1e-31
                 e + O + O2 -> O2- + O                : 1e-31
                 e + O3 + NEUTRAL -> O3- + NEUTRAL    : 1e-31
                 e + NO + NEUTRAL -> NO- + NEUTRAL    : 1e-31
                 e + N2O + NEUTRAL -> N2O- + NEUTRAL  : 1e-31
                 e + O2 + N2 -> O2- + N2              : {1.1e-31 * (300/Te)^2 * exp(-70/Tgas) * exp(1500*(Te-Tgas)/(Te*Tgas))}
                 ####
                 # electron detachment
                 ####
                 O-  + O -> O2  + e                   : 1.4e-10
                 O-  + N -> NO  + e                   : 2.6e-10
                 O-  + NO -> NO2 + e                  : 2.6e-10
                 O-  + N2 -> N2O + e                  : 5.0e-13
                 O-  + O2 -> O3  + e                  : 5.0e-15
                 O-  + O2a1 -> O3  + e                : 3.0e-10
                 O-  + O2b1 -> O   + O2 + e           : 6.9e-10
                 O-  + N2A3 -> O   + N2 + e           : 2.2e-9
                 O-  + N2B3 -> O   + N2 + e           : 1.9e-9
                 O-  + O3 -> O2  + O2 + e             : 3.0e-10
                 O2- + O -> O3  + e                   : 1.5e-10
                 O2- + N -> NO2 + e                   : 5.0e-10
                 O2- + O2 -> O2  + O2 + e             : {2.7e-10 * (TeffN2/300.0d0)^0.5 * exp(-5590/TeffN2)}
                 O2- + O2a1 -> O2  + O2 + e           : 2.0e-10
                 O2- + O2b1 -> O2  + O2 + e           : 3.6e-10
                 O2- + N2 -> O2  + N2 + e             : {1.9e-12 * (TeffN2/300.0d0)^0.5 * exp(-4990/TeffN2)}
                 O2- + N2A3 -> O2  + N2 + e           : 2.1e-9
                 O2- + N2B3 -> O2  + N2 + e           : 2.5e-9
                 O3- + O -> O2  + O2 + e              : 3.0e-10
                 ####
                 # Detachment for O3- NO- N2O- NO2- NO3- has to be verified (from inp source file)
                 ####
                 NO- + N -> N2O + e                   : 5e-10 
                 O3- + N -> NO + O2 + e               : 5e-10
                 N2O- + N -> NO + N2 + e              : 5e-10
                 NO2- + N -> NO + NO + e              : 5e-10
                 NO3- + N -> NO + NO2 + e             : 5e-10
                 NO- + O -> NO2 + e                   : 1.5e-10
                 N2O- + O -> NO + NO + e              : 1.5e-10
                 NO2- + O -> NO + O2 + e              : 1.5e-10
                 NO3- + O -> NO + O3 + e              : 1.5e-10
                 O3- + N2A3 -> O3 + N2 + e            : 2.1e-9
                 NO- + N2A3 -> NO + N2 + e            : 2.1e-9
                 N2O- + N2A3 -> N2O + N2 + e          : 2.1e-9
                 NO2- + N2A3 -> NO2 + N2 + e          : 2.1e-9
                 NO3- + N2A3 -> NO3 + N2 + e          : 2.1e-9
                 O3- + N2B3 -> O3 + N2 + e            : 2.5e-9
                 NO- + N2B3 -> NO + N2 + e            : 2.5e-9
                 N2O- + N2B3 -> N2O + N2 + e          : 2.5e-9
                 NO2- + N2B3 -> NO2 + N2 + e          : 2.5e-9
                 NO3- + N2B3 -> NO3 + N2 + e          : 2.5e-9
                 ####
                 # optical transitions and predissociation (Capitelli2000, page 157)
                 ####
                 N2A3 -> N2                           : 0.5 
                 N2B3 -> N2A3                         : 1.34e5
                 N2a_1 -> N2                          : 1e2
                 N2C3 -> N2B3                         : 2.45e7
                 O2a1 -> O2                           : 2.6e-4
                 O2b1 -> O2a1                         : 1.5e-3
                 O2b1 -> O2                           : 8.5e-2
                 O2_4ev -> O2                         : 11
                 ####
                 # quenching and excitation of N2 (Capitelli2000, page 159)
                 ####
                 N2A3 + O -> NO + N2D                 : 7e-12
                 N2A3 + O -> NO + O1S                 : 2.1e-11
                 N2A3 + N -> N2 + N                   : 2e-12
                 N2A3 + N -> N2 + N2P                 : {4.0e-11 * (300/Tgas)^0.667}
                 N2A3 + O2 -> N2 + O + O1D            : {2.1e-12 * (Tgas/300)^0.55}
                 N2A3 + O2 -> N2 + O2a1               : {2.0e-13 * (Tgas/300)^0.55}
                 N2A3 + O2 -> N2 + O2b1               : {2.0e-13 * (Tgas/300)^0.55}
                 N2A3 + O2 -> N2O + O                 : {2.0e-14 * (Tgas/300)^0.55}
                 N2A3 + N2 -> N2 + N2                 : 3e-16 
                 N2A3 + NO -> N2 + NO                 : 6.9e-11
                 N2A3 + N2O -> N2 + N + NO            : 1.0e-11
                 N2A3 + NO2 -> N2 + O + NO            : 1.0e-12
                 N2A3 + N2A3 -> N2 + N2B3             : 3e-10
                 N2A3 + N2A3 -> N2 + N2C3             : 1.5e-10
                 N2B3 + N2 -> N2A3 + N2               : 3e-11
                 N2B3 + N2 -> N2 + N2                 : 2e-12
                 N2B3 + O2 -> N2 + O + O              : 3e-10
                 N2B3 + NO -> N2A3 + NO               : 2.4e-10
                 N2C3 + N2 -> N2a_1 + N2              : 1e-11
                 N2C3 + O2 -> N2 + O + O1S            : 3e-10
                 N2a_1 + N2 -> N2B3 + N2              : 1.93e-13
                 N2a_1 + O2 -> N2 + O + O             : 2.8e-11
                 N2a_1 + NO -> N2 + N + O             : 3.6e-10
                 N2a_1 + N2A3 -> N4+ + e              : 4e-12
                 N2a_1 + N2a_1 -> N4+ + e             : 1e-11
                 N + N + N2 -> N2A3 + N2              : 1.7e-33
                 N + N + O2 -> N2A3 + O2              : 1.7e-33
                 N + N + NO -> N2A3 + NO              : 1.7e-33
                 N + N + N -> N2A3 + N                : 1e-32
                 N + N + O -> N2A3 + O                : 1e-32
                 N + N + N2 -> N2B3 + N2              : 2.4e-33
                 N + N + O2 -> N2B3 + O2              : 2.4e-33
                 N + N + NO -> N2B3 + NO              : 2.4e-33
                 N + N + N -> N2B3 + N                : 1.4e-32
                 N + N + O -> N2B3 + O                : 1.4e-32
                 ####
                 # deactivation of N metastables (Capitelli2000, page 161)
                 ####
                 N2D + O -> N + O1D                   : 4e-13
                 N2D + O2 -> NO + O                   : 5.2e-12
                 N2D + NO -> N2 + O                   : 1.8e-10
                 N2D + N2O -> NO + N2                 : 3.5e-12
                 N2D + N2 -> N + N2                   : {1.0e-13 * exp(-510/Tgas)}
                 N2P + N -> N + N                     : 1.8e-12
                 N2P + O -> N + O                     : 1.0e-12
                 N2P + N -> N2D + N                   : 6.0e-13
                 N2P + N2 -> N + N2                   : 6.0e-14
                 N2P + N2D -> N2+ + e                 : 1.0e-13
                 N2P + O2 -> NO + O                   : 2.6e-12
                 N2P + NO -> N2A3 + O                 : 3.0e-11
                 ####
                 # quenching and excitation of O2 (Capitelli2000, page 160)
                 ####
                 O2a1 + O -> O2 + O                   : 7.0e-16
                 O2a1 + N -> NO + O                   : {2.0e-14 * exp(-600/Tgas)}
                 O2a1 + O2 -> O2 + O2                 : {3.8e-18 * exp(-205/Tgas)}
                 O2a1 + N2 -> O2 + N2                 : 3.0e-21
                 O2a1 + NO -> O2 + NO                 : 2.5e-11
                 O2a1 + O3 -> O2 + O2 + O1D           : {5.2e-11 * exp(-2840/Tgas)}
                 O2a1 + O2a1 -> O2 + O2b1             : {7.0e-28 * Tgas^3.8 * exp(700/Tgas)}
                 O + O3 -> O2 + O2a1                  : {1.0e-11 * exp(-2300/Tgas)}
                 O2b1 + O -> O2a1 + O                 : 8.1e-14
                 O2b1 + O -> O2 + O1D                 : {3.4e-11 * (300/Tgas)^0.1 * exp(-4200/Tgas)}
                 O2b1 + O2 -> O2a1 + O2               : {4.3e-22 * Tgas^2.4 * exp(-281/Tgas)}
                 O2b1 + N2 -> O2a1 + N2               : {1.7e-15 * (Tgas/300)}
                 O2b1 + NO -> O2a1 + NO               : 6.0e-14
                 O2b1 + O3 -> O2 + O2 + O             : 2.2e-11
                 O24_ev + O -> O2 + O1S               : 9.0e-12
                 O24_ev + O2 -> O2b1 + O2b1           : 3.0e-13
                 O24_ev + N2 -> O2b1 + N2             : 9.0e-15
                 ####
                 # deactivation of O metastables (Capitelli2000, page 161)
                 ####
                 O1D + O -> O + O                     : 8.0e-12
                 O1D + O2 -> O + O2                   : {6.4e-12 * exp(67/Tgas)}
                 O1D + O2 -> O + O2a1                 : 1.0e-12
                 O1D + O2 -> O + O2b1                 : {2.6e-11 * exp(67/Tgas)}
                 O1D + N2 -> O + N2                   : 2.3e-11
                 O1D + O3 -> O2 + O + O               : 1.2e-10
                 O1D + O3 -> O2 + O2                  : 1.2e-10
                 O1D + NO -> O2 + N                   : 1.7e-10
                 O1D + N2O -> NO + NO                 : 7.2e-11
                 O1D + N2O -> O2 + N2                 : 4.4e-11
                 O1S + O -> O1D + O                   : 5.0e-11 * exp(-300/Tgas)}
                 O1S + N -> O + N                     : 1.0e-12
                 O1S + O2 -> O1D + O2                 : {1.3e-12 * exp(-850/Tgas)}
                 O1S + O2 -> O + O + O                : {3.0e-12 * exp(-850/Tgas)}
                 O1S + N2 -> O + N2                   : 1.0e-17
                 O1S + O2a1 -> O + O24_ev             : 1.1e-10
                 O1S + O2a1 -> O1D + O2b1             : 2.9e-11
                 O1S + O2a1 -> O + O + O              : 3.2e-11
                 O1S + NO -> O + NO                   : 2.9e-10
                 O1S + NO -> O1D + NO                 : 5.1e-10
                 O1S + O3 -> O2 + O2                  : 2.9e-10
                 O1S + O3 -> O2 + O + O1D             : 2.9e-10
                 O1S + N2O -> O + N2O                 : 6.3e-12
                 O1S + N2O -> O1D + N2O               : 3.1e-12
                 ####
                 # bimolecular nitrogen-oxygen reactions (Capitelli2000, page 168)
                 # Two missing reactions: 
                 # N + O3 -> NO + O2 : < 2.0e-16
                 # O + N2O5 -> product : < 3.0e-16
                 ####
                 N + NO -> O + N2                     : {1.8e-11 * (Tgas/300.0)^0.5}
                 N + O2 -> O + NO                     : {3.2e-12 * (Tgas/300.0) * exp(-3150/Tgas)}
                 N + NO2 -> O + O + N2                : 9.1e-13
                 N + NO2 -> O + N2O                   : 3.0e-12
                 N + NO2 -> N2 + O2                   : 7.0e-13
                 N + NO2 -> NO + NO                   : 2.3e-12
                 O + N2 -> N + NO                     : {3.0e-10 * exp(-38370/Tgas)}
                 O + NO -> N + O2                     : {7.5e-12 * (Tgas/300.0) * exp(-19500/Tgas)}
                 O + NO -> NO2                        : 4.2e-18
                 O + N2O -> N2 + O2                   : {8.3e-12 * exp(-14000/Tgas)}
                 O + N2O -> NO + NO                   : {1.5e-10 * exp(-14090/Tgas)}
                 O + NO2 -> NO + O2                   : {9.1e-12 * (Tgas/300)^0.18}
                 O + NO3 -> O2 + NO2                  : 1.0e-11
                 N2 + O2 -> O + N2O                   : {2.5e-10 * exp(-50390/Tgas)}
                 NO + NO -> N + NO2                   : {3.3e-16 * (300/Tgas)^0.5 * exp(-39200/Tgas)}
                 NO + NO -> O + N2O                   : {2.2e-12 * exp(-32100/Tgas)}
                 NO + NO -> N2 + O2                   : {5.1e-13 * exp(-33660/Tgas)}
                 NO + O2 -> O + NO2                   : {2.8e-12 * exp(-23400/Tgas)}
                 NO + O3 -> O2 + NO2                  : {2.5e-13 * exp(-765/Tgas)}
                 NO + N2O -> N2 + NO2                 : {4.6e-10 * exp(-25170/Tgas)}
                 NO + NO3 -> NO2 + NO2                : 1.7e-11
                 O2 + O2 -> O + O3                    : {2.0e-11 * exp(-49800/Tgas)}
                 O2 + NO2 -> NO + O3                  : {2.8e-12 * exp(-25400/Tgas)}
                 NO2 + NO2 -> NO + NO + O2            : {3.3e-12 * exp(-13500/Tgas)}
                 NO2 + NO2 -> NO + NO3                : {4.5e-10 * exp(-18500/Tgas)}
                 NO2 + O3 -> O2 + NO3                 : {1.2e-13 * exp(-2450/Tgas)}
                 NO2 + NO3 -> NO + NO2 + O2           : {2.3e-13 * exp(-1600/Tgas)}
                 NO3 + O2 -> NO2 + O3                 : {1.5e-12 * exp(-15020/Tgas)}
                 NO3 + NO3 -> O2 + NO2 + NO2          : {4.3e-12 * exp(-3850/Tgas)}
                 N + N -> N2+ + e                     : {2.7e-11 * exp(-6.74e4/Tgas)}
                 N + O -> NO+ + e                     : {1.6e-12 * (Tgas/300)^0.5 * (0.19d0+8.6*Tgas) * exp(-32000/Tgas)}
                 ####
                 # dissociation of nitrogen-oxygen molecules (Capitelli2000, page 169)
                 ####
                 N2 + N2 -> N + N + N2                : {5.4e-8 * (1.0-exp(-3354/Tgas)) * exp(-113200/Tgas))
                 O2 + N2 -> N + N + O2                : {5.4e-8 * (1.0-exp(-3354/Tgas)) * exp(-113200/Tgas))
                 NO + N2 -> N + N + NO                : {5.4e-8 * (1.0-exp(-3354/Tgas)) * exp(-113200/Tgas))
                 O + N2 -> N + N + O                  : {5.4e-8 * (1.0-exp(-3354/Tgas)) * exp(-113200/Tgas) * 6.6)
                 N + N2 -> N + N + N                  : {5.4e-8 * (1.0-exp(-3354/Tgas)) * exp(-113200/Tgas) * 6.6)
                 O2 + N2 -> O + O + N2                : {6.1e-9 * (1.0-exp(-2240/Tgas)) * exp(-59380/Tgas))
                 O2 + O2 -> O + O + O2                : {6.1e-9 * (1.0-exp(-2240/Tgas)) * exp(-59380/Tgas) * 5.9)
                 O2 + O -> O + O + O                  : {6.1e-9 * (1.0-exp(-2240/Tgas)) * exp(-59380/Tgas) * 21)
                 O2 + N -> O + O + N                  : {6.1e-9 * (1.0-exp(-2240/Tgas)) * exp(-59380/Tgas))
                 O2 + NO -> O + O + NO                : {6.1e-9 * (1.0-exp(-2240/Tgas)) * exp(-59380/Tgas))
                 NO + N2 -> N + O + N2                : {8.7e-9 * exp(-75994/Tgas)}
                 NO + O2 -> N + O + O2                : {8.7e-9 * exp(-75994/Tgas)}
                 NO + O -> N + O + O                  : {8.7e-9 * exp(-75994/Tgas) * 20}
                 NO + N -> N + O + N                  : {8.7e-9 * exp(-75994/Tgas) * 20}
                 NO + NO -> N + O + NO                : {8.7e-9 * exp(-75994/Tgas) * 20}
                 O3 + N2 -> O2 + O + N2               : {6.6e-10 * exp(-11600/Tgas)}
                 O3 + O2 -> O2 + O + O2               : {6.6e-10 * exp(-11600/Tgas) * 0.38}
                 O3 + N -> O2 + O + N                 : {6.6e-10 * exp(-11600/Tgas) * 6.3*exp(170/Tgas)}
                 O3 + O -> O2 + O + O                 : {6.6e-10 * exp(-11600/Tgas) * 6.3*exp(170/Tgas)}
                 N2O + N2 -> N2 + O + N2              : {1.2e-8 * (300/Tgas) * exp(-29000/Tgas)}
                 N2O + O2 -> N2 + O + O2              : {1.2e-8 * (300/Tgas) * exp(-29000/Tgas)}
                 N2O + NO -> N2 + O + NO              : {1.2e-8 * (300/Tgas) * exp(-29000/Tgas) * 2}
                 N2O + N2O -> N2 + O + N2O            : {1.2e-8 * (300/Tgas) * exp(-29000/Tgas) * 4}
                 NO2 + N2 -> N2 + O + N2              : {6.8e-6 * (300/Tgas)^2 * exp(-36180/Tgas)}
                 NO2 + O2 -> N2 + O + O2              : {6.8e-6 * (300/Tgas)^2 * exp(-36180/Tgas) * 0.78}
                 NO2 + NO -> N2 + O + NO              : {6.8e-6 * (300/Tgas)^2 * exp(-36180/Tgas) * 7.8}
                 NO2 + NO2 -> N2 + O + NO2            : {6.8e-6 * (300/Tgas)^2 * exp(-36180/Tgas) * 5.9}
                 NO3 + N2 -> NO2 + O + N2             : {3.1e-5 * (300/Tgas)^2 * exp(-25000/Tgas)}
                 NO3 + O2 -> NO2 + O + O2             : {3.1e-5 * (300/Tgas)^2 * exp(-25000/Tgas)}
                 NO3 + NO -> NO2 + O + NO             : {3.1e-5 * (300/Tgas)^2 * exp(-25000/Tgas)}
                 NO3 + N -> NO2 + O + N               : {3.1e-5 * (300/Tgas)^2 * exp(-25000/Tgas) * 10}
                 NO3 + O -> NO2 + O + O               : {3.1e-5 * (300/Tgas)^2 * exp(-25000/Tgas) * 10}
                 NO3 + N2 -> NO + O2 + N2             : {6.2e-5 * (300/Tgas)^2 * exp(-25000/Tgas)}
                 NO3 + O2 -> NO + O2 + O2             : {6.2e-5 * (300/Tgas)^2 * exp(-25000/Tgas)}
                 NO3 + NO -> NO + O2 + NO             : {6.2e-5 * (300/Tgas)^2 * exp(-25000/Tgas)}
                 NO3 + N -> NO + O2 + N               : {6.2e-5 * (300/Tgas)^2 * exp(-25000/Tgas) * 12}
                 NO3 + O -> NO + O2 + O               : {6.2e-5 * (300/Tgas)^2 * exp(-25000/Tgas) * 12}
                 N2O5 + NEUTRAL -> NO2 + NO3 + NEUTRAL : {2.1e-11 * (300/Tgas)^4.4 * exp(-11080/Tgas)}
                 ####
                 # recombination of nitrogen-oxygen molecules (Capitelli, page 170)
                 # note "max" rate coefficients in the source file.
                 # Do not know how to implement something similar in CRANE.
                 ####
                 N + N + N2 -> N2 + N2                : {8.3e-34 * exp(500/Tgas)}
                 N + N + O2 -> N2 + O2                : {1.8e-33 * exp(435/Tgas)}
                 N + N + NO -> N2 + NO                : {1.8e-33 * exp(435/Tgas)}
                 N + N + N -> N2 + N                  : {1.8e-33 * exp(435/Tgas) * 3}
                 N + N + O -> N2 + O                  : {1.8e-33 * exp(435/Tgas) * 3}
                 O + O + N2 -> O2 + N2                : {2.8e-34 * exp(720/Tgas)}
                 O + O + O2 -> O2 + O2                : {4.0e-33 * (300/Tgas)^0.41}
                 O + O + N -> O2 + N                  : {4.0e-33 * (300/Tgas)^0.41 * 0.8}
                 O + O + O -> O2 + O                  : {4.0e-33 * (300/Tgas)^0.41 * 3.6}
                 O + O + NO -> O2 + NO                : {4.0e-33 * (300/Tgas)^0.41 * 0.17}
                 N + O + N2 -> NO + N2                : {1.0e-32 * (300/Tgas)^0.5}
                 N + O + O2 -> NO + O2                : {1.0e-32 * (300/Tgas)^0.5}
                 N + O + N -> NO + N                  : {1.8e-31 * (300/Tgas)}
                 N + O + O -> NO + O                  : {1.8e-31 * (300/Tgas)}
                 N + O + NO -> NO + NO                : {1.8e-31 * (300/Tgas)}
                 O + O2 + N2 -> O3 + N2               : {5.8e-34 * (300/Tgas)^2.8}
                 O + O2 + O2 -> O3 + O2               : {7.6e-34 * (300/Tgas)^1.9}
                 O + O2 + NO -> O3 + NO               : {7.6e-34 * (300/Tgas)^1.9}
                 O + O2 + N -> O3 + N                 : {3.9e-33 * (300/Tgas)^1.9}
                 O + O2 + O -> O3 + O                 : {3.9e-33 * (300/Tgas)^1.9}
                 O + N2 + NEUTRAL -> N2O + NEUTRAL    : {3.9e-35 * exp(-10400/Tgas)}
                 O + NO + N2 -> NO2 + N2              : {1.2e-31 * (300/Tgas)^1.8}
                 O + NO + O2 -> NO2 + O2              : {1.2e-31 * (300/Tgas)^1.8 * 0.78}
                 O + NO + NO -> NO2 + NO              : {1.2e-31 * (300/Tgas)^1.8 * 0.78}
                 O + NO2 + N2 -> NO3 + N2             : {8.9e-32 * (300/Tgas)^2}
                 O + NO2 + O2 -> NO3 + O2             : {8.9e-32 * (300/Tgas)^2}
                 O + NO2 + N -> NO3 + N               : {8.9e-32 * (300/Tgas)^2 * 13}
                 O + NO2 + O -> NO3 + O               : {8.9e-32 * (300/Tgas)^2 * 13}
                 O + NO2 + NO -> NO3 + NO             : {8.9e-32 * (300/Tgas)^2 * 2.4}
                 NO2 + NO3 + NEUTRAL -> N2O5 + NEUTRAL : {3.7e-30 * (300/Tgas)^4.1}
                 ####
                 # positive ion reactions (Capitelli, 179)
                 ####
                 N+ + O   -> N + O^+                              : 1.0e-12
                 N+ + O2  -> O2+ + N                              : 2.8e-10
                 N+ + O2  -> NO+ + O                              : 2.5e-10
                 N+ + O2  -> O+ + NO                              : 2.8e-11
                 N+ + O3  -> NO+ + O2                             : 5.0e-10
                 N+ + NO  -> NO+ + N                              : 8.0e-10
                 N+ + NO  -> N2+ + O                              : 3.0e-12
                 N+ + NO  -> O+ + N2                              : 1.0e-12
                 N+ + N2O -> NO+ + N2                             : 5.5e-10
                 O+ + N2 -> NO+ + N                               : {( 1.5 - 2.0e-3*TeffN + 9.6e-7*TeffN^2 ) * 1.0e-12}
                 O+ + O2 -> O2+ + O                               : {2.0e-11 * (300/TeffN)^0.5}
                 O+ + O3 -> O2+ + O2                              : 1.0e-10
                 O+ + NO -> NO+ + O                               : 2.4e-11
                 O+ + NO -> O2+ + N                               : 3.0e-12
                 O+ + N2D -> N+ + O                               : 1.3e-10
                 O+ + N2O -> NO+ + NO                             : 2.3e-10
                 O+ + N2O -> N2O+ + O                             : 2.2e-10
                 O+ + N2O -> O2+ + N2                             : 2.0e-11
                 O+ + NO2 -> NO2+ + O                             : 1.6e-9
                 N2+ + O2 -> O2+ + N2                             : {6.0e-11 * (300/TeffN2)^0.5}
                 N2+ + O  -> NO+ + N                              : {1.3e-10 * (300/TeffN2)^0.5}
                 N2+ + O3 -> O2+ + O + N2                         : 1.0e-10
                 N2+ + N  -> N+ + N2                              : {7.2e-13 * (TeffN2/300)}
                 N2+ + NO -> NO+ + N2                             : 3.3e-10
                 N2+ + N2O -> N2O+ + N2                           : 5.0e-10
                 N2+ + N2O -> NO+ + N + N2                        : 4.0e-10
                 O2+ + N2 -> NO+ + NO                             : 1.0e-17
                 O2+ + N  -> NO+ + O                              : 1.2e-10
                 O2+ + NO -> NO+ + O2                             : 6.3e-10
                 O2+ + NO2 -> NO+ + O3                            : 1.0e-11
                 O2+ + NO2 -> NO2+ + O2                           : 6.6e-10
                 N3+ + O2 -> O2+ + N + N2                         : 2.3e-11
                 N3+ + O2 -> NO2+ + N2                            : 4.4e-11
                 N3+ + N  -> N2+ + N2                             : 6.6e-11
                 N3+ + NO -> NO+ + N + N2                         : 7.0e-11
                 N3+ + NO -> N2O+ + N2                            : 7.0e-11
                 NO2+ + NO -> NO+ + NO2                           : 2.9e-10
                 N2O+ + NO -> NO+ + N2O                           : 2.9e-10
                 N4+ + N2 -> N2+ + N2 + N2                        : 1.0e-10
                 N4+ + O2 -> O2+ + N2 + N2                        : 2.5e-10
                 N4+ + O  -> O+ + N2 + N2                         : 2.5e-10
                 N4+ + N  -> N+ + N2 + N2                         : 1.0e-11
                 N4+ + NO -> NO+ + N2 + N2                        : 4.0e-10
                 O4+ + N2 -> O2+N2 + O2                           : {4.6e-12 * (TeffN4/300)^2.5 * exp(-2650/TeffN4)}
                 O4+ + O2 -> O2+ + O2 + O2                        : {3.3e-6  * (300/TeffN4)^4   * exp(-5030/TeffN4)}
                 O4+ + O2a1 -> O2+ + O2 + O2                      : 1.0e-10
                 O4+ + O2b1 -> O2+ + O2 + O2                      : 1.0e-10
                 O4+ + O      -> O2+ + O3                         : 3.0e-10
                 O4+ + NO     -> NO+ + O2 + O2                    : 1.0e-10
                 O2pN2 + N2 -> O2+ + N2 + N2                      : {1.1e-6 * (300/TeffN4)^5.3 * exp(-2360/TeffN4)}
                 O2pN2 + O2 -> O4+ + N2                           : 1.0e-9
                 N+ + N2 + N2 -> N3+ + N2                         : {1.7e-29 * (300/TeffN)^2.1}
                 N+ + O + ANY_NEUTRAL -> NO+ + ANY_NEUTRAL        : 1.0e-29
                 N+ + N + ANY_NEUTRAL -> N2+ + ANY_NEUTRAL        : 1.0e-29
                 O+ + N2 + ANY_NEUTRAL -> NO+ + N + ANY_NEUTRAL   : {6.0e-29 * (300/TeffN)^2}
                 O+ + O  + ANY_NEUTRAL -> O2+ + ANY_NEUTRAL       : 1.0e-29
                 O+ + N  + ANY_NEUTRAL -> NO+ + ANY_NEUTRAL       : 1.0e-29
                 N2+ + N2 + N2 -> N4+ + N2                        : {5.2e-29 * (300/TeffN2)^2.2}
                 N2+ + N  + N2 -> N3+ + N2                        : {9.0e-30 * exp(400/TeffN2)}
                 O2+ + O2 + O2 -> O4+ + O2                        : {2.4e-30 * (300/TeffN2)^3.2}
                 O2+ + N2 + N2 -> O2pN2 + N2                      : {9.0e-31 * (300/TeffN2)^2}
                 ####
                 # negative ion reactions (Capitelli, 182-183)
                 # NOTE missing reactions: 
                 # O2^- + N2O    => O3^-  + N2                       ! < 1.0d-12
                 #O3^- + N2     => NO2^- + NO                       ! < 5.0d-14
                 ####
                 O-   + O2a1 -> O2-  + O                          : 1.0e-10
                 O-   + O3 -> O3-  + O                            : 8.0e-10
                 O-   + NO2 -> NO2- + O                           : 1.2e-9
                 O-   + N2O -> NO-  + NO                          : 2.0e-10
                 O-   + N2O -> N2O- + O                           : 2.0e-12
                 O2-  + O -> O-   + O2                            : 3.3e-10
                 O2-  + O3 -> O3-  + O2                           : 3.5e-10
                 O2-  + NO2 -> NO2- + O2                          : 7.0e-10
                 O2-  + NO3 -> NO3- + O2                          : 5.0e-10
                 O3-  + O -> O2-  + O2                            : 1.0e-11
                 O3-  + NO -> NO3- + O                            : 1.0e-11
                 O3-  + NO -> NO2- + O2                           : 2.6e-12
                 O3-  + NO2 -> NO2- + O3                          : 7.0e-11
                 O3-  + NO2 -> NO3- + O2                          : 2.0e-11
                 O3-  + NO3 -> NO3- + O3                          : 5.0e-10
                 NO-  + O2 -> O2-  + NO                           : 5.0e-10
                 NO-  + NO2 -> NO2- + NO                          : 7.4e-10
                 NO-  + N2O -> NO2- + N2                          : 2.8e-14
                 NO2- + O3 -> NO3- + O2                           : 1.8e-11
                 NO2- + NO2 -> NO3- + NO                          : 4.0e-12
                 NO2- + NO3 -> NO3- + NO2                         : 5.0e-10
                 NO2- + N2O5 -> NO3- + NO2 + NO2                  : 7.0e-10
                 NO3- + NO -> NO2- + NO2                          : 3.0e-15
                 O4- + N2 -> O2- + O2 + N2                        : {1.0e-10 * exp(-1044/TeffN4)}
                 O4- + O2 -> O2- + O2 + O2                        : {1.0e-10 * exp(-1044/TeffN4)}
                 O4- + O -> O3-  + O2                             : 4.0e-10
                 O4- + O -> O-   + O2  + O2                       : 3.0e-10
                 O4- + O2a1 -> O2-  + O2  + O2                    : 1.0e-10
                 O4- + O2b1 -> O2-  + O2  + O2                    : 1.0e-10
                 O4- + NO -> NO3- + O2                            : 2.5e-10
                 O-  + O2 + ANY_NEUTRAL -> O3- + ANY_NEUTRAL      : {1.1e-30 * (300.0d0/TeffN)}
                 O-  + NO + ANY_NEUTRAL -> NO2- + ANY_NEUTRAL     : 1.0e-29
                 O2- + O2 + ANY_NEUTRAL -> O4- + ANY_NEUTRAL      : {3.5e-31 * (300.0d0/TeffN2)}
                 ####
                 # ion-ion recombination (Kossyi1992)
                 ####
                 O- + N+ -> O + N                                 : {2e-7 * (300/TionN)^0.5}
                 O- + N2+ -> O + N2                               : {2e-7 * (300/TionN)^0.5}
                 O- + O+ -> O + O                                 : {2e-7 * (300/TionN)^0.5}
                 O- + O2+ -> O + O2                               : {2e-7 * (300/TionN)^0.5}
                 O- + NO+ -> O + NO                               : {2e-7 * (300/TionN)^0.5}
                 O- + N2O+ -> O + N2O                             : {2e-7 * (300/TionN)^0.5}
                 O- + NO2+ -> O + NO2                             : {2e-7 * (300/TionN)^0.5}

                 O2- + N+ -> O + N                                 : {2e-7 * (300/TionN)^0.5}
                 O2- + N2+ -> O + N2                               : {2e-7 * (300/TionN)^0.5}
                 O2- + O+ -> O + O                                 : {2e-7 * (300/TionN)^0.5}
                 O2- + O2+ -> O + O2                               : {2e-7 * (300/TionN)^0.5}
                 O2- + NO+ -> O + NO                               : {2e-7 * (300/TionN)^0.5}
                 O2- + N2O+ -> O + N2O                             : {2e-7 * (300/TionN)^0.5}
                 O2- + NO2+ -> O + NO2                             : {2e-7 * (300/TionN)^0.5}

                 O3- + N+ -> O + N                                 : {2e-7 * (300/TionN)^0.5}
                 O3- + N2+ -> O + N2                               : {2e-7 * (300/TionN)^0.5}
                 O3- + O+ -> O + O                                 : {2e-7 * (300/TionN)^0.5}
                 O3- + O2+ -> O + O2                               : {2e-7 * (300/TionN)^0.5}
                 O3- + NO+ -> O + NO                               : {2e-7 * (300/TionN)^0.5}
                 O3- + N2O+ -> O + N2O                             : {2e-7 * (300/TionN)^0.5}
                 O3- + NO2+ -> O + NO2                             : {2e-7 * (300/TionN)^0.5}

                 NO- + N+ -> O + N                                 : {2e-7 * (300/TionN)^0.5}
                 NO- + N2+ -> O + N2                               : {2e-7 * (300/TionN)^0.5}
                 NO- + O+ -> O + O                                 : {2e-7 * (300/TionN)^0.5}
                 NO- + O2+ -> O + O2                               : {2e-7 * (300/TionN)^0.5}
                 NO- + NO+ -> O + NO                               : {2e-7 * (300/TionN)^0.5}
                 NO- + N2O+ -> O + N2O                             : {2e-7 * (300/TionN)^0.5}
                 NO- + NO2+ -> O + NO2                             : {2e-7 * (300/TionN)^0.5}
                 N2O- + N+ -> O2 + N                                 : {2e-7 * (300/TionN)^0.5}
                 N2O- + N2+ -> O2 + N2                               : {2e-7 * (300/TionN)^0.5}
                 N2O- + O+ -> O2 + O                                 : {2e-7 * (300/TionN)^0.5}
                 N2O- + O2+ -> O2 + O2                               : {2e-7 * (300/TionN)^0.5}
                 N2O- + NO+ -> O2 + NO                               : {2e-7 * (300/TionN)^0.5}
                 N2O- + N2O+ -> O2 + N2O                             : {2e-7 * (300/TionN)^0.5}
                 N2O- + NO2+ -> O2 + NO2                             : {2e-7 * (300/TionN)^0.5}
                 NO2- + N+ -> O3 + N                                 : {2e-7 * (300/TionN)^0.5}
                 NO2- + N2+ -> O3 + N2                               : {2e-7 * (300/TionN)^0.5}
                 NO2- + O+ -> O3 + O                                 : {2e-7 * (300/TionN)^0.5}
                 NO2- + O2+ -> O3 + O2                               : {2e-7 * (300/TionN)^0.5}
                 NO2- + NO+ -> O3 + NO                               : {2e-7 * (300/TionN)^0.5}
                 NO2- + N2O+ -> O3 + N2O                             : {2e-7 * (300/TionN)^0.5}
                 NO2- + NO2+ -> O3 + NO2                             : {2e-7 * (300/TionN)^0.5}
                 NO3- + N+ -> NO3 + N                                 : {2e-7 * (300/TionN)^0.5}
                 NO3- + N2+ -> NO3 + N2                               : {2e-7 * (300/TionN)^0.5}
                 NO3- + O+ -> NO3 + O                                 : {2e-7 * (300/TionN)^0.5}
                 NO3- + O2+ -> NO3 + O2                               : {2e-7 * (300/TionN)^0.5}
                 NO3- + NO+ -> NO3 + NO                               : {2e-7 * (300/TionN)^0.5}
                 NO3- + N2O+ -> NO3 + N2O                             : {2e-7 * (300/TionN)^0.5}
                 NO3- + NO2+ -> NO3 + NO2                             : {2e-7 * (300/TionN)^0.5}
                 O- + N2+ -> O + N + N                              : 1e-7
                 O- + N3+ -> O + N + N2                             : 1e-7
                 O- + N4+ -> O + N2 + N2                            : 1e-7
                 O- + O2+ -> O + O + O                              : 1e-7
                 O- + O4+ -> O + O2 + O2                            : 1e-7
                 O- + NO+ -> O + N + O                              : 1e-7
                 O- + N2O+ -> O + N2 + O                            : 1e-7
                 O- + NO2+ -> O + N + O2                            : 1e-7
                 O- + O2pN2 -> O + O2 + N2                          : 1e-7
                 O2- + N2+ -> O2 + N + N                              : 1e-7
                 O2- + N3+ -> O2 + N + N2                             : 1e-7
                 O2- + N4+ -> O2 + N2 + N2                            : 1e-7
                 O2- + O2+ -> O2 + O + O                              : 1e-7
                 O2- + O4+ -> O2 + O2 + O2                            : 1e-7
                 O2- + NO+ -> O2 + N + O                              : 1e-7
                 O2- + N2O+ -> O2 + N2 + O                            : 1e-7
                 O2- + NO2+ -> O2 + N + O2                            : 1e-7
                 O2- + O2pN2 -> O2 + O2 + N2                          : 1e-7
                 O3- + N2+ -> O3 + N + N                              : 1e-7
                 O3- + N3+ -> O3 + N + N2                             : 1e-7
                 O3- + N4+ -> O3 + N2 + N2                            : 1e-7
                 O3- + O2+ -> O3 + O + O                              : 1e-7
                 O3- + O4+ -> O3 + O2 + O2                            : 1e-7
                 O3- + NO+ -> O3 + N + O                              : 1e-7
                 O3- + N2O+ -> O3 + N2 + O                            : 1e-7
                 O3- + NO2+ -> O3 + N + O2                            : 1e-7
                 O3- + O2pN2 -> O3 + O2 + N2                          : 1e-7
                 NO- + N2+ -> NO + N + N                              : 1e-7
                 NO- + N3+ -> NO + N + N2                             : 1e-7
                 NO- + N4+ -> NO + N2 + N2                            : 1e-7
                 NO- + O2+ -> NO + O + O                              : 1e-7
                 NO- + O4+ -> NO + O2 + O2                            : 1e-7
                 NO- + NO+ -> NO + N + O                              : 1e-7
                 NO- + N2O+ -> NO + N2 + O                            : 1e-7
                 NO- + NO2+ -> NO + N + O2                            : 1e-7
                 NO- + O2pN2 -> NO + O2 + N2                          : 1e-7
                 N2O- + N2+ -> N2O + N + N                            : 1e-7
                 N2O- + N3+ -> N2O + N + N2                           : 1e-7
                 N2O- + N4+ -> N2O + N2 + N2                          : 1e-7
                 N2O- + O2+ -> N2O + O + O                            : 1e-7
                 N2O- + O4+ -> N2O + O2 + O2                          : 1e-7
                 N2O- + NO+ -> N2O + N + O                            : 1e-7
                 N2O- + N2O+ -> N2O + N2 + O                          : 1e-7
                 N2O- + NO2+ -> N2O + N + O2                          : 1e-7
                 N2O- + O2pN2 -> N2O + O2 + N2                        : 1e-7
                 NO2- + N2+ -> NO2 + N + N                            : 1e-7
                 NO2- + N3+ -> NO2 + N + N2                           : 1e-7
                 NO2- + N4+ -> NO2 + N2 + N2                          : 1e-7
                 NO2- + O2+ -> NO2 + O + O                            : 1e-7
                 NO2- + O4+ -> NO2 + O2 + O2                          : 1e-7
                 NO2- + NO+ -> NO2 + N + O                            : 1e-7
                 NO2- + N2O+ -> NO2 + N2 + O                          : 1e-7
                 NO2- + NO2+ -> NO2 + N + O2                          : 1e-7
                 NO2- + O2pN2 -> NO2 + O2 + N2                        : 1e-7
                 NO3- + N2+ -> NO3 + N + N                            : 1e-7
                 NO3- + N3+ -> NO3 + N + N2                           : 1e-7
                 NO3- + N4+ -> NO3 + N2 + N2                          : 1e-7
                 NO3- + O2+ -> NO3 + O + O                            : 1e-7
                 NO3- + O4+ -> NO3 + O2 + O2                          : 1e-7
                 NO3- + NO+ -> NO3 + N + O                            : 1e-7
                 NO3- + N2O+ -> NO3 + N2 + O                          : 1e-7
                 NO3- + NO2+ -> NO3 + N + O2                          : 1e-7
                 NO3- + O2pN2 -> NO3 + O2 + N2                        : 1e-7
                 O4- + N+ -> O2 + O2 + N                              : 1e-7 
                 O4- + N2+ -> O2 + O2 + N2                            : 1e-7 
                 O4- + O+ -> O2 + O2 + O                              : 1e-7 
                 O4- + O2+ -> O2 + O2 + O2                            : 1e-7 
                 O4- + NO+ -> O2 + O2 + NO                            : 1e-7 
                 O4- + N2O+ -> O2 + O2 + N2O                          : 1e-7 
                 O4- + NO2+ -> O2 + O2 + NO2                          : 1e-7 
                 O4- + N3+ -> O2 + O2 + N2 + N                        : 1e-7 
                 O4- + N4+ -> O2 + O2 + N2 + N2                       : 1e-7 
                 O4- + O4+ -> O2 + O2 + O2 + O2                       : 1e-7 
                 O4- + O2pN2 -> O2 + O2 + O2 + N2                     : 1e-7 
                 O- + N+ + NEUTRAL -> O + N + NEUTRAL                 : {2e-25 * (300/TionN)^2.5}
                 O- + N2+ + NEUTRAL -> O + N2 + NEUTRAL               : {2e-25 * (300/TionN)^2.5}
                 O- + O+ + NEUTRAL -> O + O + NEUTRAL                 : {2e-25 * (300/TionN)^2.5}
                 O- + O2+ + NEUTRAL -> O + O2 + NEUTRAL               : {2e-25 * (300/TionN)^2.5}
                 O- + NO+ + NEUTRAL -> O + NO + NEUTRAL               : {2e-25 * (300/TionN)^2.5}
                 O2- + N+ + NEUTRAL -> O2 + N + NEUTRAL               : {2e-25 * (300/TionN)^2.5}
                 O2- + N2+ + NEUTRAL -> O2 + N2 + NEUTRAL             : {2e-25 * (300/TionN)^2.5}
                 O2- + O+ + NEUTRAL -> O2 + O + NEUTRAL               : {2e-25 * (300/TionN)^2.5}
                 O2- + O2+ + NEUTRAL -> O2 + O2 + NEUTRAL             : {2e-25 * (300/TionN)^2.5}
                 O2- + NO+ + NEUTRAL -> O2 + NO + NEUTRAL             : {2e-25 * (300/TionN)^2.5}
                 O- + N+ + NEUTRAL -> NO + NEUTRAL                    : {2e-25 * (300/TionN)^2.5}
                 O- + N2+ + NEUTRAL -> N2O + NEUTRAL                  : {2e-25 * (300/TionN)^2.5}
                 O- + O+ + NEUTRAL -> O2 + NEUTRAL                    : {2e-25 * (300/TionN)^2.5}
                 O- + O2+ + NEUTRAL -> O3 + NEUTRAL                   : {2e-25 * (300/TionN)^2.5}
                 O- + NO+ + NEUTRAL -> NO2 + NEUTRAL                  : {2e-25 * (300/TionN)^2.5}
                 O2- + N+ + NEUTRAL -> NO2 + NEUTRAL                  : {2e-25 * (300/TionN)^2.5}
                 O2- + O+ + NEUTRAL -> O3 + NEUTRAL                   : {2e-25 * (300/TionN)^2.5}
                 O2- + NO+ + NEUTRAL -> NO3 + NEUTRAL                 : {2e-25 * (300/TionN)^2.5}
                 ####
                 # Three-body recombination of O3- NO- N2O- NO2- NO3- has to be verified
                 ####
                 O3- + N+ + NEUTRAL -> O3 + N + NEUTRAL               : {2e-25 * (300/TionN2)^2.5}
                 O3- + N2+ + NEUTRAL -> O3 + N2 + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 O3- + O+ + NEUTRAL -> O3 + O + NEUTRAL               : {2e-25 * (300/TionN2)^2.5}
                 O3- + O2+ + NEUTRAL -> O3 + O2 + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 O3- + NO+ + NEUTRAL -> O3 + NO + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 O3- + N2O+ + NEUTRAL -> O3 + N2O + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 O3- + NO2+ + NEUTRAL -> O3 + NO2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO- + N+ + NEUTRAL -> NO + N + NEUTRAL               : {2e-25 * (300/TionN2)^2.5}
                 NO- + N2+ + NEUTRAL -> NO + N2 + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO- + O+ + NEUTRAL -> NO + O + NEUTRAL               : {2e-25 * (300/TionN2)^2.5}
                 NO- + O2+ + NEUTRAL -> NO + O2 + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO- + NO+ + NEUTRAL -> NO + NO + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO- + N2O+ + NEUTRAL -> NO + N2O + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO- + NO2+ + NEUTRAL -> NO + NO2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 N2O- + N+ + NEUTRAL -> N2O + N + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 N2O- + N2+ + NEUTRAL -> N2O + N2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 N2O- + O+ + NEUTRAL -> N2O + O + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 N2O- + O2+ + NEUTRAL -> N2O + O2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 N2O- + NO+ + NEUTRAL -> N2O + NO + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 N2O- + N2O+ + NEUTRAL -> N2O + N2O + NEUTRAL         : {2e-25 * (300/TionN2)^2.5}
                 N2O- + NO2+ + NEUTRAL -> N2O + NO2 + NEUTRAL         : {2e-25 * (300/TionN2)^2.5}
                 NO2- + N+ + NEUTRAL -> NO2 + N + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO2- + N2+ + NEUTRAL -> NO2 + N2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO2- + O+ + NEUTRAL -> NO2 + O + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO2- + O2+ + NEUTRAL -> NO2 + O2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO2- + NO+ + NEUTRAL -> NO2 + NO + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO2- + N2O+ + NEUTRAL -> NO2 + N2O + NEUTRAL         : {2e-25 * (300/TionN2)^2.5}
                 NO2- + NO2+ + NEUTRAL -> NO2 + NO2 + NEUTRAL         : {2e-25 * (300/TionN2)^2.5}
                 NO3- + N+ + NEUTRAL -> NO3 + N + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO3- + N2+ + NEUTRAL -> NO3 + N2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO3- + O+ + NEUTRAL -> NO3 + O + NEUTRAL             : {2e-25 * (300/TionN2)^2.5}
                 NO3- + O2+ + NEUTRAL -> NO3 + O2 + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO3- + NO+ + NEUTRAL -> NO3 + NO + NEUTRAL           : {2e-25 * (300/TionN2)^2.5}
                 NO3- + N2O+ + NEUTRAL -> NO3 + N2O + NEUTRAL         : {2e-25 * (300/TionN2)^2.5}
                 NO3- + NO2+ + NEUTRAL -> NO3 + NO2 + NEUTRAL         : {2e-25 * (300/TionN2)^2.5}'



    #reactions = 'N + O2 -> NO + O                     : {1.4995e-14 * Tgas * exp(-3.26992e3/Tgas)}
    #             N + NO -> N2 + O                     : {1.8731e-11 * Tgas^0.14}
    #             N + OH -> NO + H                     : 6.31e-11
    #             NO + O + NEUTRAL -> NO2 + NEUTRAL    : {2.603e-23 * Tgas^(-2.87)*exp(-7.80494e2/Tgas)}
    #             NO2 + H -> NO + OH                   : {2.507e-10 * exp(-1.82166e2/Tgas)}
    #             NO2 + O -> NO + O2                   : {1.7436e-10 * Tgas^(-0.5200)}
    #             NO + HO2 -> NO2 + OH                 : {3.487e-12 * exp(2.501e2/Tgas)}
    #             NO2 + HO2 -> HNO2 + O2               : {3.072e-23 * Tgas^3.26 * exp(-2.50754e3/Tgas)}
    #             NO2 + HO2 -> HNO2 + O2               : {3.1716e-24 * Tgas^3.32 * exp(-1.5318e3/Tgas)}
    #             NO2 + NO2 -> NO3 + NO                : {1.5941e-14 * Tgas^0.73 * exp(-1.05173e4/Tgas)}
    #             NO2 + H2 -> HNO2 + H                 : {3.9687e-22 * Tgas^3.15 * exp(-1.56328e4/Tgas)}
    #             NO2 + H2 -> HONO + H                 : {1.9179e-24 * Tgas^3.89 * exp(-1.30736e4/Tgas)}
    #             NO2 + NO2 -> NO + NO + O2            : {7.4724e-12 * exp(-1.38884e4/Tgas)}
    #             NO2 + O + NEUTRAL -> NO3 + NEUTRAL   : {6.8935e-28 * Tgas^-1.5)}
    #             NO3 + H -> NO2 + OH                  : 9.9632e-11
    #             NO3 + O -> NO2 + O2                  : 1.6605e-11
    #             NO3 + OH -> NO2 + HO2                : 2.3248e-11
    #             NO3 + HO2 -> NO2 + O2 + OH           : 2.4908e-12
    #             NO3 + NO2 -> NO + NO2 + O2           : {8.3027e-14 * exp(-1.47947e3/Tgas)}
    #             NO + H + O2 -> HNO + O2              : {2.4908e-9 * Tgas^(-1.75) * 0.5} 
    #             NO + H + H2 -> HNO + H2              : {2.4908e-9 * Tgas^(-1.75)}
    #             NO + H + H2O -> HNO + H2O            : {2.4908e-9 * Tgas^(-1.75)* 9.0}
    #             NO + H + Ar -> HNO + Ar              : {2.4908e-9 * Tgas^(-1.75)* -2.5e-1}
    #             NO + H + NEUTRAL -> HNO + NEUTRAL    : {2.4908e-9 * Tgas^(-1.75)}
    #             HNO + H -> NO + H2                   : {7.3064e-13 * Tgas^0.72 * exp(-3.27093e2/Tgas)}
    #             HNO + O -> NO + OH                   : 3.8192e-11
    #             HNO + OH -> NO + H2O                 : 5.9779e-11
    #             HNO + NO2 -> NO + HONO               : {7.3396e-20 * Tgas^2.64 * exp(-2.03301e3/Tgas)}
    #             HNO + HNO -> N2O + H2O               : {1.4945e-15 * exp(-1.55998e3/Tgas)}
    #             HON + NEUTRAL -> NO + H + NEUTRAL    : {8.4687e-5 * Tgas^(-1.73) * exp(-8.07416e3/Tgas)} 
    #             HON + H -> HNO + H                   : 3.9853e-11
    #             HON + H -> NH + OH                   : 1.6605e-11
    #             HON + O -> NO + OH                   : 1.1624e-10
    #             HON + OH -> HONO + H                 : 6.6422e-11
    #             HON + O2 -> NO2 + OH                 : {1.6605e-12 * exp(-2.5e3/Tgas)}
    #             # HONO sub-mechanism [Zhang2017]
    #             '
  [../]
[]
