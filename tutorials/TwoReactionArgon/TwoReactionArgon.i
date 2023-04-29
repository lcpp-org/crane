# This tutorial is a simple example of a plasma chemical kinetics model
# It includes two reactions: ionization and recombination
# The ionization rate coefficients are obtained as a function of the reduced field
# by solving the electron Boltzmann equation using LoKI-B and the Morgan database@

# here, we set a single mesh element because we are interested in a 
# global/volume-averaged density and to emphasize the chemistry rather than transport
[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]

# here we introduce all of our plasma species: electrons, argon ions, and argon neutrals
# you can name these however you like, but the concise "e" is preferable to "electrons"
# here you can set the initial density of each species
[Variables]
  [e]
    family = SCALAR
    order = FIRST
    initial_condition = 1 #cm^-3
  []

  [Ar+]
    family = SCALAR
    order = FIRST
    initial_condition = 1 #cm^-3
  []

  [Ar]
    family = SCALAR
    order = FIRST
    initial_condition = 2.5e19 #cm^-3
    scaling = 2.5e-19
    # 1 atm/760 torr at 295 K
    # the scaling factor is introduced in order to make the values of the densities more similar
    # necessary for a smooth numerical solution and convergence, but does not effect final result.
  []
[]

# with our variables listed, we also provide a time-derivative kernel for each one 
# Chemical kinetics is essentially a giant set of first-order ODEs!
[ScalarKernels]
  [de_dt]
    type = ODETimeDerivative
    variable = e
  []

  [dAr+_dt]
    type = ODETimeDerivative
    variable = Ar+
  []

  [dAr_dt]
    type = ODETimeDerivative
    variable = Ar
  []
[]

# here we introduce the ionization and recombination reaction for our two-reaction system
[ChemicalReactions]
  [ScalarNetwork]
    species = 'e Ar+ Ar'
    # the species are listed as a single string with spaces to separate them
    file_location = 'data'
    # any look-up tables of rate coefficients are located in the directory called "data"
    sampling_variable = 'reduced_field' 
    # these rate coefficients are tabulated against the reduced electric field in Td
    interpolation_type = 'spline'
    # we use spline interpolation to obtain values of k when the exact E/N is not tabulated

    # the reactions are listed as a single string and colons to separate the reaction and its rate coefficient
    # ionization rate is in cm3/s and recombination rate in cm6/s
    reactions = 'e + Ar -> e + e + Ar+         : EEDF (ionization)
                 e + Ar+ + Ar -> Ar + Ar       : 1e-25'
    # The usage of 'EEDF' after the colon signifies that this rate coefficient is tabulated in a .txt file called "ionization.txt"
    # Otherwise, exact expressions can be used
   []
[]

# here we introduce any variable that does not need a time derivative in order to compute
# in this case, we introduce the reduced electric field (E/N) that heats the electrons
[AuxVariables]
  [reduced_field]
    order = FIRST
    family = SCALAR
    initial_condition = 30 #Td
    # since there are no AuxKernels operating on reduced_field, it stays constant at 30
  []
[]

# We setup the Executioner to solve for a transient problem
# Note that the dt used is incredibly sensitive to the reaction rates!
[Executioner]
  type = Transient
  end_time = 0.75e-6 #s
  solve_type = 'newton'
  dt = 1e-10 #s
  dtmin = 1e-20
  dtmax = 1e-8
  petsc_options_iname = '-snes_linesearch_type'
  petsc_options_value = 'basic'
[]

# Preconditing options 
# Learn more at: https://mooseframework.inl.gov/syntax/Preconditioning/index.html
[Preconditioning]
  active = 'smp'

  [smp]
    type = SMP
    full = true
  []

  [fd]
    type = FDP
    full = true
  []
[]

# Defines the output file type and name
# CSV file types are perfect for visualization 0D problems with Python
[Outputs]
  [out]
    type = CSV
  []
[]
