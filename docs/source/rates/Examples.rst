Examples
========

LoKI-B: Find the Ionization Rate of Argon
-----------------------------------------

This example shows how to use LoKI-B to find the ionization rate of argon
using the cross sections from the LXCat database.

The steps to find the ionization rate coefficient are as follows:

1. Download the electron-impact cross-sections of interest from a database (e.g. LXCat).
2. Use LoKI-B to solve the electron Boltzmann equation and obtain the ionization rate coefficient.
3. Tabulate the ionization rate coefficient in a CRANE-acceptable format.

LoKI-B solves the Electron Boltzmann Equation (EBE) and obtains the ionization rate coefficient
:math:`k_i` as a function of the reduced electric field :math:`E/N`.
The EBE is solved for a range of :math:`E/N` values from 0.001 to 1000 Td.
The resulting rate coefficients are then tabulated by LoKI-B, 
and we pre-process these into a CRANE-acceptable format. 

Our computational tool of choice is `LoKI-B <https://github.com/IST-Lisbon/LoKI>`_, 
which requires MATLAB. If you do not have access to MATLAB, you can use 
`BOLSIG+ <https://nl.lxcat.net/solvers/BolsigPlus/index.php>`_. 
The proper usage of LoKI-B or BOLSIG+ is not discussed here.

Obtain Cross Sections from LxCat
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The first step is to save the electron-impact cross-sections of interest in a
tabulated format. The cross-sections can be obtained from the online `LXCat database
<https://www.lxcat.net>`_. The LXCat database contains a large number of
electron-impact cross-sections for a wide range of species. The cross-sections
are tabulated in a ``.txt`` file, which can be downloaded from the LXCat website.


The ``.txt`` LXCat file contains a header with information about the cross-sections,
and the cross-sections of each process are tabulated separately. 
Each table contains the cross-section :math:`\sigma` (:math:`\text{m}^2`) in the second column
as a function of electron energy :math:`\varepsilon` (:math:`eV`) in the first column for the given process.
For our simple problem, we nominally need just the ionization cross-section. 
However, we will also include the metastable excitation cross-section,
and importantly, the elastic momentum transfer cross-section.
The elastic momentum transfer cross-section is needed to calculate the
electron energy loss rate, and therfore, necessary to obtain an accurate EEDF.

LxCat has multiple databases for Ar. Here, we use the 
`Morgan database <https://www.lxcat.net/Morgan>`_. 
From the Morgan database, we select cross-sections for the following processes:

* ``e + Ar -> e + Ar``: elastic momentum transfer
* ``e + Ar -> e + Ar*``: metastable excitation
* ``e + Ar -> e + e + Ar+``: ionization
  
The cross-sections are tabulated in a ``.txt`` file, which can be downloaded
from the LxCat website. We have saved the ``.txt`` file in the directory
``crane/tutorials/TwoReactionArgon/data`` as ``Ar_Morgan.txt``. 
The file is modified such that: (a) only the metastable
excitation pathway is included (i.e. we exclude the "total excitation"
process), (b) Ar* is renamed to Ar(eff), and (c) the first comment of each
process describes the reaction from the ground state Ar(1S0), which is parsed
by LoKI-B.

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: text
   :lines: 50-65
   :caption: Ar_Morgan.txt (lines 50-65)

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: text
   :lines: 124-138
   :caption: Ar_Morgan.txt (lines 124-138)

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_Morgan.txt
   :language: text
   :lines: 155-168
   :caption: Ar_Morgan.txt (lines 155-168)

.. warning:: 

    In this example, we are only interested in obtaining 
    the ionization rate coefficient from LoKI-B. However, this DOES NOT 
    mean the ionization cross section should be the only one tabulated. 
    The other cross sections, especially elastic scattering, are absolutely
    necessary to accurately reflect the relaxation of the electrons.


Using LoKI-B to Calculate the Ionization Rate Coefficient
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

In this section, we set up a LoKI-B input file named ``Ar_lumped.in``
that provides the necessary information of operating conditions and cross-sections
to solve the EBE and obtain the ionization rate coefficient using LoKI-B. 
This LoKI-B input file ``Ar_lumped.in`` that was used for this tutorial can be found in 
``crane/tutorials/TwoReactionArgon/data/Ar_lumped.in``. 
Please note that CRANE does not read or write this file in any way;
``Ar_lumped.in`` must be copied into an appropriate location with LoKI-B, 
an external software to CRANE. 

The input file is reported below. For an explanation on the functionality 
and use of each of the working conditions, electron kinetics configurations, outputs, etc., 
please refer to the LoKI-B documentation. However, a brief explanation can be provided. 
As shown, we have set the range of reduced electric field values to 0.001 to 1000,
the excitation frequency is set to 0 for a direct-current field,
the gas pressure is 101325 Pa = 1 atm, the gas temperature is 295 K, 
the electron kinetics calculation is set up to solve the electron Boltzmann equation, 
the input file points to the ``Ar_Morgan.txt`` file that contains the desired cross-sections,
the population of the gases and states is listed, and after execution, the program outputs various 
data files, including the rate coefficients for each process.

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/Ar_lumped.in
   :language: matlab  
   :lines: 1-
   :caption: Ar_lumped.in

Place both ``Ar_lumped.in`` and the cross section file ``Ar_Morgan.txt`` 
in the directory ``LoKI/Code/Input/Argon``, and run LoKI-B in MATLAB with the command 

.. code:: matlab 
    
   >> lokibcl('Argon/Argon_lumped.in')

while in ``LoKI/Code`` to execute the input file.

After the input file is executed, a new directory ``LoKI/Output/ArLumped`` is generated 
which includes the output lookup table ``lookUpTableRateCoeff.txt``.
This file contains the rate coefficients in units of :math:`\text{m}^3/\text{s}` 
for each process versus the reduced electric field in Td.

.. literalinclude:: ../../tutorials/TwoReactionArgon/data/lookUpTableRateCoeff.txt
   :language: python  
   :lines: 1-16
   :caption: lookUpTableRateCoeff.txt (lines 1-16)


Tabulate the Ionization Rate Coefficient in CRANE Format
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To tabulate the ionization rate coefficient in a CRANE-acceptable format,
the following Python script is a suitable example:

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