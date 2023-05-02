Two-Reaction Argon Plasma
==========================

Problem Description
-------------------

Here, we consider a simple argon plasma composed of only three species, 
electrons (:math:`\text{e}`), argon ions (:math:`\text{Ar}^{+}`), 
and argon neutrals (:math:`\text{Ar}`). The three species interact
through two reactions:

.. math::
    :label: two_reaction_mechanism

    \text{e} + \text{Ar} \to \text{e} + \text{e} + \text{Ar}^{+} 

    \text{e} + \text{Ar}^{+} + \text{Ar} \to \text{Ar} + \text{Ar} 

where the first reaction is ionization and the second reaction is
recombination. The rate coefficients of these reactions are denoted
:math:`k_i` and :math:`k_r`, respectively.

The goal of this tutorial is to demonstrate how to use CRANE to solve for the
time evolution of the plasma density and compare the results
to the expected analytical steady-state value, :math:`n_p = k_i / k_r`.

We will also use this tutorial to demonstrate how to pre-process the rate
coefficientd of the two reactions in the system and how to use tabulated
rate coefficients in CRANE. The pre-processing of the rate coefficients is
performed using the `LoKI-B <https://github.com/IST-Lisbon/LoKI>`_ code
from the IST-Lisbon group.

The tutorial is organized as follows:

1. We first solve the electron Boltzmann equation (EBE) to obtain the ionization rate
   coefficient :math:`k_i` as a function of the reduced electric field :math:`E/N`.

2. We then use CRANE to solve the system of ordinary differential equations 
   (ODEs) that describe the time evolution of the density of each species. 

3. Finally, we compare the steady-state plasma density :math:`n_p` obtained from 
   CRANE to the analytical value :math:`n_p = k_i / k_r`.


Theory
------

For each our three species under consideration (:math:`\text{e}`, 
:math:`\text{Ar}^{+}`, :math:`\text{Ar}`), we can write an ordinary differential 
equation that describes the change in density due to the plasma-chemical reactions:

.. math::
    :label: ODE_system

    \frac{d n_e}{d t} = k_i n_e n_{Ar} - k_r n_e n_i n_{Ar},

    \frac{d n_i}{d t} = k_i n_e n_{Ar} - k_r n_e n_i n_{Ar},
    
    \frac{d n_{Ar}}{d t} = - k_i n_e n_{Ar} + k_r n_e n_i n_{Ar},

where :math:`n_e`, :math:`n_i`, :math:`n_{Ar}` are the number densities 
(:math:`\text{cm}^{-3}`) of the electrons, ions, and neutral gas, respectively, 
and :math:`k_i` (:math:`\text{cm}^3/\text{s}`) and :math:`k_r` (:math:`\text{cm}^6/\text{s}`) 
are the rate coefficients of ionization and recombination. 
It is immediately noticable that the equations describing the chemical kinetics 
of the electrons and the ions are identical. 
When combined with enforcing of identical initial conditions on the two species, 
this ensures that the quasineutrality of the plasma is maintained, i.e. :math:`n_e = n_i`.

As part of the verification of this model, we can predict the steady-state 
plasma density :math:`n_p = n_e = n_i` by setting the first (or second) equation 
from the set above to zero, and solving for :math:`n_p`

.. math::
    :label: tworeaction_equilibrium_density

    \frac{d n_p}{d t} = k_i n_p n_{Ar} - k_r n_p n_p n_{Ar} = 0 \Rightarrow n_p = \frac{k_i}{k_r}.

The equilibrium plasma density :math:`n_p` is determined by the ratio of
the ionization and recombination rate coefficients, :math:`k_i` and :math:`k_r` 
respectively. This result will be used to verify the results obtained from CRANE.


Simulation Conditions 
---------------------

The neutral argon gas is initially at atmospheric pressure, 
:math:`p_{Ar}` = 1 atm = 760 torr and room temperature, 
:math:`T_{Ar}` = 295 K. The corresponding neutral gas density is
obtained from the ideal gas law, :math:`n_{Ar} = p_{Ar}/(k_B T_{Ar})`, 
where :math:`k_B` is the Boltzmann constant, which gives an initial 
neutral gas density of :math:`n_{Ar} = 2.5 \times 10^{19} \; \text{cm}^{-3}`. 
Conversely, the ions and electrons start from a very low initial density of 
:math:`n_{e,i}(t=0) = 1 \; \text{cm}^{-3}`. A reduced electric field :math:`E/N = 30` Td is 
kept constant throughout the simulation to achieve breakdown and sustain the plasma to steady-state. 

In order to calculate the value of the ionization rate coefficient :math:`k_i`, 
we can either (1) solve the EBE or (2) integrate the ionization cross-section over an assumed 
electron energy distribution function (EEDF), such as a Maxwellian.
We opt for the former as this is the most common practice, 
which we will use going forward also in other tutorials. On the other hand,
the recombination rate coefficient :math:`k_r` of the second reaction 
is assumed to be constant, :math:`k_r = 10^{-25} \; \text{cm}^6/\text{s}`. 

For the solution of the electron Boltzmann equation (EBE), we use the
solver `LoKI-B <https://github.com/IST-Lisbon/LoKI>`_ to obtain the
rate coefficient :math:`k_i` as a function of the reduced electric field
:math:`E/N`. The EBE is solved for a range of :math:`E/N` values from 0.001 to 1000 Td.
The resulting rate coefficients are then tabulated by LoKI-B, 
and we pre-process these into a CRANE-acceptable format. 

Summarizing the simulation conditions in a table:

+---------------------+-------------------+ 
| Parameter           | Value             |
+=====================+===================+
| Initial Ar density  | 2.5e19 cm^-3      |
+---------------------+-------------------+ 
| Initial e density   | 1 cm^-3           | 
+---------------------+-------------------+ 
| Initial i density   | 1 cm^-3           |
+---------------------+-------------------+ 
| Reduced field E/N   | 30 Td             | 
+---------------------+-------------------+ 
| Ionization from EEDF| 2.13e-25 cm^3/s   | 
+---------------------+-------------------+ 
| Recombination k_r   | 1e-25 cm^6/s      | 
+---------------------+-------------------+ 


Find the Ionization Rate Coefficient using LoKI-B
---------------------------------------------------

In this section, we solve the EBE and obtain the ionization rate coefficient
:math:`k_i` as a function of the reduced electric field :math:`E/N`.
The EBE is solved for a range of :math:`E/N` values from 0.001 to 1000 Td.
The resulting rate coefficients are then tabulated by LoKI-B, 
and we pre-process these into a CRANE-acceptable format. 

Our computational tool of choice is `LoKI-B <https://github.com/IST-Lisbon/LoKI>`_, 
which requires MATLAB. If you do not have access to MATLAB, you can use 
`BOLSIG+ <https://nl.lxcat.net/solvers/BolsigPlus/index.php>`_. 
The proper usage of LoKI-B or BOLSIG+ will not discussed here.

Cross-sections from LxCat
^^^^^^^^^^^^^^^^^^^^^^^^^

The first step is to save the electron-impact cross-sections of interest in a
tabulated format. The cross-sections can be obtained from the `LXCat database
<https://www.lxcat.net>`_. The LXCat database contains a large number of
electron-impact cross-sections for a wide range of species. The cross-sections
are tabulated in a ``.txt`` file, which can be downloaded from the LxCat website.


The ``.txt`` LxCat file contains a header with information about the cross-sections, 
and the cross-sections of each process are tabulated separately. 
Each table contains the cross section :math:`\sigma` (:math:`\text{m}^2`) in the second column
as a function of electron energy :math:`\varepsilon` (:math:`eV`) in the first column for the given process.


The process names are in the format ``<species><process>``, where ``<species>`` is
the species name and ``<process>`` is the process name. For example, the
process name ``Ar*`` corresponds to the metastable excitation process of
argon. The process names are not standardized, and the same process can have
different names in different databases. The process names are parsed by LoKI-B
to identify the cross-sections of interest. The cross-sections of interest are
specified in the ``LoKI-B.inp`` file, which is read by LoKI-B. The cross-sections
of interest are specified in the ``LoKI-B.inp`` file by the process names. The
process names are specified in the ``LoKI-B.inp`` file in the format
``<species><process>``, where ``<species>`` is the species name and
``<process>`` is the process name. 

For our simple problem, we nominally need just the ionization cross-section. 
However, we will also include the metastable excitation cross-section,
and importantly, the elastic momentum transfer cross-section.
The elastic momentum transfer cross-section is needed to calculate the
electron energy loss rate, which is needed to solve the EBE.

LxCat has multiple databases for Ar. Here, we use the 
`Morgan database <https://www.lxcat.net/Morgan>`_. 
From the Morgan database we select cross-sections for the following processes:

* ``Ar*``: metastable excitation
* ``Ar+``: ionization
* ``Ar``: elastic momentum transfer
  
The cross-sections are tabulated in a ``.txt`` file, which can be downloaded
from the LxCat website. We have saved the ``.txt`` file in the directory
``crane/tutorials/TwoReactionArgon/data``. The file is renamed to
``Ar_Morgan.txt``. The file is modified such that: (a) only the metastable
excitation pathway is included (i.e. we exclude the "total excitation"
process) and is renamed from Ar* to Ar(eff), and (b) the first comment of each
process describes the reaction from the ground state Ar(1S0), which is parsed
by LoKI-B.

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: bash 
   :lines: 50-65
   :caption: Ar_Morgan.txt (lines 50-65)

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: bash 
   :lines: 124-138
   :caption: Ar_Morgan.txt (lines 124-138)

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: bash 
   :lines: 155-168
   :caption: Ar_Morgan.txt (lines 155-168)

.. warning:: 

    In this example, we are only interested in obtaining 
    the ionization rate coefficient from LoKI-B. However, this DOES NOT 
    mean the ionization cross section should be the only one tabulated. 
    The other cross sections, especially elastic scattering, are absolutely
    necessary to accurately reflect the relaxation of the electrons.


Use LoKI-B to find the ionization rate coefficient
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Set up a LoKI-B input file ``Ar_lumped.in`` to solve for the EBE with the provided 
cross sections and operating conditions, and obtain the ionization rate coefficient.

The LoKI-B input file ``Ar_lumped.in`` that was used for this tutorial can 
be found in ``crane/tutorials/TwoReactionArgon/data/Ar_lumped.in``. 
The input file is reported below. The input file is divided into sections.
The first section is the ``Species`` section, which specifies the species
properties. The second section is the ``Processes`` section, which specifies

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_lumped.in
   :language: matlab  
   :lines: 1-
   :caption: Ar_lumped.in

Place both ``Ar_lumped.in`` and the cross section file ``Ar_Morgan.txt`` 
in the directory ``LoKI/Code/Input/Argon``, and run LoKI in Matlab with the command 

.. code:: matlab 
    
   >> lokibcl('Argon/Argon_lumped.in')

while in ``LoKI/Code`` will execute the input file.

After the input file is executed, a new directory ``LoKI/Output/ArLumped`` is generated 
which includes the output lookup table ``lookUpTableRateCoeff.txt``.
This file contains the rate coefficients in units of :math:`\text{m}^3/\text{s}` 
for each process versus the reduced electric field in Td.

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/lookUpTableRateCoeff.txt
   :language: python  
   :lines: 1-16
   :caption: lookUpTableRateCoeff.txt (lines 1-16)


Tabulate the ionization rate coefficient in CRANE format
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Tabulate the ionization rate coefficient in a CRANE-acceptable format, 
for example using the following Python script 

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/RateCoeffReader.py
   :language: python
   :lines: 1-
   :caption: RateCoeffReader.py

This writes a new two-column file ``ionization.txt``, parsable by CRANE,
where the first column is the reduced electric field in Td, and the second column
is the ionization rate coefficient in :math:`\text{cm}^3/\text{s}`. 

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/ionization.txt
   :lines: 56-66
   :caption: ionization.txt (lines 56-66)

Now that we have found the ionization rate coefficient and saved it in a file, 
we are ready to build the CRANE input file for our problem. 


Input File
-----------

The input file ``TwoReactionArgon.i`` in ``crane/tutorials/TwoReactionArgon`` is shown below:

.. literalinclude:: ../../tutorials/TwoReactionArgon/TwoReactionArgon.i
   :language: python 
   :lines: 1-

(Follows a description of the input file sections)

Running 
--------

Run CRANE with the command

.. code-block:: bash  

    $ ../../crane-opt -i TwoReactionArgon.i

The output file ``TwoReactionArgon_out.csv`` is generated, which tabulates 
the value of each variable including rate coefficients for each timestep.

.. .. literalinclude:: ../../tutorials/TwoReactionArgon/TwoReactionArgon_out.csv
..    :language: python
..    :lines: 1-10
..    :caption: TwoReactionArgon_out.csv (lines 1-10)


Visualizing the Results
-------------------------

After the input file is excuted, the file ``TwoReactionArgon_out.csv`` is generated, 
which tabulates the value of each variable including rate coefficients for each timestep.
We can now plot the electron density :math:`n_e(t)` as a function of time, and 
compare it with the steady-state prediction as solved in the Theory section.

.. literalinclude:: ../../tutorials/TwoReactionArgon/plasma_density_plot.py
   :language: python
   :lines: 1-

As expected, the plasma density reaches the predicted steady-state value:

.. figure:: figures/plasma_density.png
    :align: center
    :width: 500px
    :alt: 
    :figclass: align-center

    The plasma density :math:`n_e` as a function of time. 
    The blue horizontal line is the expected steady-state value, 
    the red line is the numerical solution calculated by CRANE.

