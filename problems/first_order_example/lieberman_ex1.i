# This input file describes a simple system of two first-order reactions
#
# Based on the example shown in the textbook: 
#
# Lieberman, Michael A. and Lichtenberg, Allan J. "Principles of Plasma 
# Discharges and Materials Processing." Second Edition. John Wiley & Sons, Inc. 
# (2005)
#
# Chapter 9, Section 2, "First-Order Consecutive Reactions)
# Pages 290-291

[Mesh]
  # 'Dummy' mesh - a mesh is always needed to run MOOSE, but
  # scalar variables do not exist on a mesh.
  # When you're doing a 0D problem you just need to give MOOSE 
  # a single element to work with.
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax = 1
  nx = 1
[]

[Variables]
  # ODE variables
  # Global variables are called "SCALAR" in MOOSE.
  [A]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  []

  [B]
    family = SCALAR
    order = FIRST
    initial_condition = 0
  []

  [C]
    family = SCALAR
    order = FIRST
    initial_condition = 0
  []
[]

[ScalarKernels]
  # For global problems the only kernels needed are time derivatives.
  # All the source and sink terms are added by the GlobalReactions block.

  # Time derivatives:
  [dA_dt]
    type = ODETimeDerivative
    variable = A
  []
  [dB_dt]
    type = ODETimeDerivative
    variable = B
  []
  [dC_dt]
    type = ODETimeDerivative
    variable = C
  []
[]

[GlobalReactions]
  [lieberman_reactions]
    species = 'A B C'
    reactions = 'A -> B       : 1
                 B -> C       : 5'
  []
[]

[Executioner]
  type = Transient
  start_time = 0
  end_time = 3
  dt = 0.1

  solve_type = linear

  # You can select different time integrators to see how they affect
  # the accuracy of the solution
  [TimeIntegrator]
    # This one is the default
    type = ImplicitEuler

    #type = LStableDirk2
    #type = BDF2
    #type = CrankNicolson
    #type = ImplicitMidpoint
  []
[]

[Preconditioning]
  [smp]
    type = SMP
    full = true
  []
[]

[Outputs]
  [out_ka_1_kb_5]
    type = CSV
  []

  # This next one is optional, but highly recommended.
  # Without this, scalar variables are printed on the terminal as the 
  # simulation runs. 
  # It's best to keep this on to prevent things from being cluttered.
  [console]
    type = Console
    execute_scalars_on = 'none'
  []
[]
