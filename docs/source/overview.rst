Overview 
========

`CRANE <https://github.com/lcpp-org/crane>`_ 
is a software package for the simulation of the chemical kinetics of 
nonequilibrium plasmas. It is designed to be used either as a standalone 
application, or as a coupled application within the MOOSE finite-element 
framework. CRANE is written in C++ and is released under the 
`LGPL-2.1 License v3.0 <https://github.com/lcpp-org/crane/blob/master/LICENSE>`_.

CRANE was developed within the MOOSE framework. MOOSE (Multiphysics
Object Oriented Simulation Environment) is a finite
element software developed at Idaho National Laboratory (Gaston 2009), 
which provides a software framework for coupled nonlinear multiphysics simulations. 
Its modular structure allows different codes to be coupled in multiple
different ways: they may be compiled together and included in a single
fully coupled nonlinear solver, or applications can be loosely coupled
with the ``MultiApp`` system. The MOOSE environment has previously 
been used to study a wide variety of physical
applications such as superconductivity, incompressible Navier-Stokes 
systems, and large-scale nuclear reactor simulations. 
The MOOSE framework is well suited to the
study of plasmas due to its massively parallel operation and its ability
to natively treat multi-phase systems (such as plasma-liquid
interactions). 

CRANE aims to further expand the plasma simulation capabilities of the
MOOSE framework by including a chemical kinetics solver. Using the MOOSE
framework's *Actions* system, CRANE allows lists of chemical reactions
to be added to the input file in a simple human-readable format to
construct reaction networks. It may be run alone as a 0D model or it can
leverage the coupling capabilities of the framework and be compiled into
other MOOSE applications as a submodule, providing separately-developed
software with the capability to solve reaction networks. Using CRANE
alone requires no C++ programming from the user, and directly coupling
applications requires only modifying the main application's Makefile and
adding two lines to the applications main C++ file. In this way all of
the functionality of CRANE becomes available to the parent application
through the text-based input files used by MOOSE applications.

The ability to couple different codes together represents a significant
advantage both for CRANE and for MOOSE applications in general. By
compiling `Zapdos <https://shannon-lab.github.io/zapdos/>`_ 
and CRANE together, a user is able to simulate a
fully-coupled system of multispecies drift-diffusion-reaction equations.
A large number of reactions may be easily added to a Zapdos-CRANE
simulation without writing any code, allowing a user to model a plasma
discharge with necessarily large reaction networks, such as nitrogen and
oxygen plasmas. Since the dimensionality
and parallel capabilities of a problem are handled internally by MOOSE,
a coupled Zapdos-CRANE model may be easily scaled into multiple
dimensions and parallelized from the input file and command line. Future
plasma-relevant software that is developed in the MOOSE framework will
have the ability to be coupled together with Zapdos and CRANE as well.



Chemical Kinetics
-----------------

CRANE was developed to study the problem of chemical kinetics in
nonequilibrium, multi-fluid plasmas. The mathematical kernel of the
software is a coupled system of ODE rate equations:

.. math:: \frac{d n_i}{dt} = \sum_{j=1}^{j_{max}} S_{ij}

where :math:`n_i` is the concentration of species :math:`i`, and the
right hand side is the sum over all :math:`j` source and sink terms of
that species. For a two-body reaction :math:`j` between species A, B,
and C, with stoichiometric coefficients a\ :math:`_1`, a\ :math:`_2`, b,
and c, and rate coefficient :math:`k`:

.. math:: a_1 A + bB \xrightarrow{k} a_2 A + cC

The reaction has an associated reaction rate :math:`R_j`:

.. math:: R_j = k_j [A]^{a_1} [B]^b

and the rate of production for each species may then be calculated from
the reaction rate and stoichiometric coefficients:

.. math::

   \begin{aligned}
   S_A &= (a_2 - a_1)R_j \\
   S_B &= -b k_j R_j \\
   S_C &= c R_j
   \end{aligned}

Under the adiabatic isometric approximation, the gas temperature may be
changed as a result of each reaction’s associated change in enthalpy,
:math:`\delta \epsilon_j`:

.. math:: \frac{N_{gas}}{\gamma-1} \frac{dT}{dt} = \sum_{j=1}^{j_{max}} \pm \delta \epsilon_j R_j

where :math:`N_{gas}` is the neutral gas density and :math:`\gamma` is
the specific gas heat ratio.


.. _`sec:moose`:

User Interface
--------------

An input file for CRANE largely follows the same syntax as all MOOSE
applications. The input file requires at minimum five “blocks" (Mesh,
Variables, Kernels, Executioner, Outputs). Variables declares each
nonlinear variable in the system (in CRANE’s case, these are the species
densities), Kernels represent a single term or piece of physics in the
system of equations being solved, the Executioner dictates the numerical
scheme and timestepping parameters, and the output file format is
dictated in the Outputs block. When used alone, CRANE solves a global
system of ODEs and uses only scalar variables (denoted by the
``family = SCALAR`` parameter) and ScalarKernels. The smallest possible
mesh, a point, is used to ensure full compatibility with MOOSE. An
example input file is shown below.

.. code-block:: toml

   [Mesh]
      type = GeneratedMesh
      dim = 1
      nx = 1
   []

   [Variables]
      [./e]
         family = SCALAR
         order = FIRST
         initial_condition = 1
      [../]

      [./Ar]
         family = SCALAR
         order = FIRST
         initial_condition = 2.5e19
         scaling = 1e-19
      [../]

      [./Ar+]
         family = SCALAR
         order = FIRST
         initial_condition = 1
      [../]
   []

   [ScalarKernels]
      [./de_dt]
         type = ODETimeDerivative
         variable = e
      [../]

      [./dAr_dt]
         type = ODETimeDerivative
         variable = Ar
      [../]

      [./dAr+_dt]
         type = ODETimeDerivative
         variable = Ar+
      [../]
   []

   [ChemicalReactions]
      [./ScalarNetwork]
         species = 'e Ar Ar+'
         file_location = 'example_folder'
         sampling_variable = 'reduced_field'

         reactions = 'e + Ar -> e + e + Ar+   : EEDF (rxn1.txt)
                     e + Ar+ + Ar -> Ar + Ar : 1e-25'

      [../]
   []

   [AuxVariables]
      [./reduced_field]
         order = FIRST
         family = SCALAR
         initial_condition = 50e-21
      [../]
   []


   [Executioner]
      type = Transient
      end_time = 0.28e-6
      dt = 1e-9
      solve_type = NEWTON
      line_search = basic
   []

   [Preconditioning]
      [./smp]
         type = SMP
         full = true
      [../]
   []

   [Outputs]
      csv = true
      interval = 10
   []

While in principle every term in the system of equations must be
included as a Kernel (or ScalarKernel), CRANE was developed utilizing
the *Actions* system in the MOOSE framework to automatically add a
system of reactions, which is shown in the ``ChemicalReactions`` block.
This example adds six source and sink ScalarKernels to the solver
automatically without requiring the user to individually add each term
to the ScalarKernels block. In this example, the nonlinear species are
named in the ``species`` parameter (electrons, neutral argon, and
ionized argon), and the reactions (separated by a return character) are
listed in the ``reactions`` parameter. Rate coefficients are separated
from each reaction by a colon character. The first reaction’s rate
coefficient is indicated to be tabulated in a file named ‘rxn1.txt’
located in the ‘example_folder’ directory. The ``sampling_variable``
parameter dictates what such rate coefficients are tabulated with; in
this case it is tabulated as a function of the ``reduced_field``
parameter, which is an ``AuxVariable`` with a constant value of 50 Td.

As part of the MOOSE framework, CRANE has access to a wide array of
options for tuning a simulation. Solver options such as numerical
schemes, adaptive timestepping, and PETSc options are denoted in the
``Executioner`` block. MOOSE includes multiple explicit and implicit
time integrators: available implicit methods are backward Euler, Crank
Nicolson, BDF2, DIRK, and Newmark-:math:`\beta`, while the available
explicit methods are forward Euler, Midpoint, and total
variation-diminishing Runge-Kutta second order method. Note that this is
only intended to be a brief summary of options relevant to CRANE. A
detailed list of all input file options are available on the MOOSE
framework website: https://mooseframework.inl.gov

.. _`sec:coupling`:

Code Coupling
-------------

The largest advantage that CRANE has over similar chemistry solvers such
as ZDPlasKin and CHEMKIN is that it may be natively coupled to other
separately-developed MOOSE applications, without requiring additional
coding from the user. For example, when coupled to the low temperature
plasma transport code, Zapdos, all of the functionality built into CRANE
becomes natively accessible by Zapdos through the application's input
file. No data transfer is necessary in this case since the codes are
compiled together and treated as a single application. In this way the
problem becomes a fully coupled system of drift-diffusion-reaction
equations.

