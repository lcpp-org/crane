LoKI-B
=======

In this tutorial we show how to obtain rate coefficients from 
`LoKI-B <https://github.com/IST-Lisbon/LoKI>`_
and format them for use in CRANE. 
`LoKI-B <https://github.com/IST-Lisbon/LoKI>`_ is written in MATLAB. 
If you do not have access to MATLAB, you can use 
`BOLSIG+ <https://nl.lxcat.net/solvers/BolsigPlus/index.php>`_. 
BOLSIG+ is a standalone executable that can be run 
in Windows and Linux operating systems. 

We will use the example of argon, for the simple case of ionization and metastable excitation.
We will show how to solve the EBE and obtain the ionization rate coefficient
:math:`k_i` as a function of the reduced electric field :math:`E/N`
for the simple problem of an argon plasma made of three speices only. 
The resulting rate coefficients are tabulated and saved in a ``.txt`` file, 
which is read by CRANE.


Step 1. Cross-sections from LxCat
-----------------------------------------

The first step is to save the electron-impact cross-sections of interest in a
tabulated format. The cross-sections can be obtained from the `LxCat database
<https://www.lxcat.net>`_. The LxCat database contains a large number of
electron-impact cross-sections for a wide range of species. The cross-sections
are tabulated in a ``.txt`` file, which can be downloaded from the LxCat website.

The ``.txt`` LxCat file contains a header with information about the cross-sections, 
and the cross-sections are tabulated in a table. The table contains the electron
energy in eV, the total cross-section in m^2, and the cross-sections for
different processes in m^2. The first column of the table contains the
electron energy in eV, and the first row contains the process names. The
process names are in the format ``<species><process>``, where ``<species>`` is
the species name and ``<process>`` is the process name. 

For example, the process name ``Ar*`` corresponds to the metastable excitation process 
of argon. The process names are not standardized, and the same process can have
different names in different databases. The process names are parsed by LoKI-B
to identify the cross-sections of interest. The cross-sections of interest are
specified in the ``LoKI-B.inp`` file, which is read by LoKI-B. The cross-sections
of interest are specified in the ``LoKI-B.inp`` file by the process names. The
process names are specified in the ``LoKI-B.inp`` file in the format
``<species><process>``, where ``<species>`` is the species name and
``<process>`` is the process name. 

A minimal set of cross-sections for a species includes a specific process  ionization
cross-section, the elastic momentum transfer cross-section, and a loss 
channel such as metastable excitation. The ionization cross-section is needed

However, we will also include the metastable excitation cross-section,
and importantly, the elastic momentum transfer cross-section.
The elastic momentum transfer cross-section is needed to calculate the
electron energy loss rate, which is needed to solve the EBE.

LxCat has multiple databases for Ar. Here, we use the 
`Morgan database <https://www.lxcat.net/Morgan>`_. 
From the Morgan database we select cross-sections for the following processes:

* ``Ar``: elastic momentum transfer
* ``Ar*``: metastable excitation
* ``Ar+``: ionization
  
The cross-sections are tabulated in a ``.txt`` file, which can be downloaded
from the LxCat website. We have saved the ``.txt`` file in the directory
``crane/tutorials/TwoReactionArgon/data``. The file is renamed to
``Ar_Morgan.txt``. The file is modified such that: (a) only the metastable
excitation pathway is included (i.e. we exclude the "total excitation"
process) and is renamed from Ar* to Ar(eff), and (b) the first comment of each
process describes the reaction from the ground state Ar(1S0), which is parsed
by LoKI-B.

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: bash 
   :lines: 50-65
   :caption: Ar_Morgan.txt (lines 50-65)

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: bash 
   :lines: 124-138
   :caption: Ar_Morgan.txt (lines 124-138)

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: bash 
   :lines: 155-168
   :caption: Ar_Morgan.txt (lines 155-168)

.. warning:: 

    In this example, we are only interested in obtaining 
    the ionization rate coefficient from LoKI-B. However, this DOES NOT 
    mean the ionization cross section should be the only one tabulated. 
    The other cross sections, especially elastic scattering, are absolutely
    necessary to accurately reflect the relaxation of the electrons.


Step 2. Run LoKI
-------------------

Set up a LoKI-B input file ``Ar_lumped.in`` to solve for the EBE with the provided 
cross sections and operating conditions, and obtain the ionization rate coefficient.

The LoKI-B input file ``Ar_lumped.in`` that was used for this tutorial can 
be found in ``crane/tutorials/TwoReactionArgon/data/Ar_lumped.in``. 
The input file is reported below. The input file is divided into sections.
The first section is the ``Species`` section, which specifies the species
properties. The second section is the ``Processes`` section, which specifies

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/Ar_lumped.in
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

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/lookUpTableRateCoeff.txt
   :language: python  
   :lines: 1-16
   :caption: lookUpTableRateCoeff.txt (lines 1-16)


Step 3. Tabulate the rate coefficient in CRANE format
------------------------------------------------------

Tabulate the ionization rate coefficient in a CRANE-acceptable format, 
for example using the following Python script 

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/RateCoeffReader.py
   :language: python
   :lines: 1-
   :caption: RateCoeffReader.py

This writes a new two-column file ``ionization.txt``, parsable by CRANE,
where the first column is the reduced electric field in Td, and the second column
is the ionization rate coefficient in :math:`\text{cm}^3/\text{s}`. 

.. literalinclude:: ../../../tutorials/TwoReactionArgon/data/ionization.txt
   :lines: 56-66
   :caption: ionization.txt (lines 56-66)

