Running
=======

Running a CRANE input file follows the syntax

.. code-block:: bash
    
    <path-from-intput-to-crane-opt> -i <input-file>

while in the MOOSE environment. 
The file ``crane-opt`` is generated upon successful compilation of CRANE, 
and is located alongside the MAKEFILE. 
For example, to want to run ``argon_test.i`` located in ``crane/problems/argon_microdischarge``,
you would enter the following command while in ``argon_microdischarge`` directory:

.. code-block:: bash
    
    ../../crane-opt -i argon_test.i

Each set of ``../`` denotes a step up through the directories. 
Since ``argon_microdischarge`` is within ``problems``, which is within ``crane``,
we need two sets of ``../`` to be in the directory in which ``crane-opt`` is located.

To run an input file on multiple processors, 
one would add the following command before the path to `crane-opt``:

.. code-block:: bash
    
    mpiexec -np n <path-from-intput-to-crane-opt> -i <input-file>

where ``n`` is the number of processors you wish to use. For the ``argon_microdischarge`` example,
you might run

.. code-block:: bash
    
    mpiexec -np 4 ../../crane-opt -i argon_test.i

to run the input file using four cores.