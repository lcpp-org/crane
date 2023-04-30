Arrhenius
=========


Arrhenius equation
------------------

The Arrhenius equation gives the dependence of a rate coefficient 
of a reaction on the absolute temperature :math:`T` as

.. math:: k(T) = A T^{n} \exp\left(-\frac{E_a}{k_B T}\right)

where :math:`A` is the pre-exponential (frequency) factor, 
:math:`n` is the temperature exponent, :math:`E_a` is the activation
energy, and :math:`k_B` is the Boltzmann constant.
The Arrhenius equation is used to describe the temperature dependence
of the rate coefficient of a chemical reaction. 

The Arrhenius equation is also used to describe the temperature
dependence of the diffusion coefficient, the viscosity, and 
thermal conductivity of a gas:

.. math:: 
    
    D(T) = D_0 \exp\left(-\frac{E_D}{R T}\right)

    \mu(T) = \mu_0 \exp\left(\frac{E_\mu}{R T}\right)

    k(T) = k_0 \exp\left(-\frac{E_k}{R T}\right)

where 
:math:`D_0` is the diffusion coefficient at infinite temperature, 
:math:`E_D` is the activation energy for diffusion, 
:math:`\mu_0` is the viscosity at infinite temperature, 
:math:`E_\mu` is the activation energy for viscosity, 
:math:`k_0` is the thermal conductivity at infinite temperature, and 
:math:`E_k` is the activation energy for thermal conductivity.

For a review of the Arrhenius equation, and a determination of the 
parameters for various reactions, read the following 
`chapter <https://chem.libretexts.org/Bookshelves/Physical_and_Theoretical_Chemistry_Textbook_Maps/Supplemental_Modules_(Physical_and_Theoretical_Chemistry)/Kinetics/06%3A_Modeling_Reaction_Kinetics/6.02%3A_Temperature_Dependence_of_Reaction_Rates/6.2.03%3A_The_Arrhenius_Law/6.2.3.01%3A_Arrhenius_Equation>`_.


Arrhenius reactions in CRANE 
----------------------------

To specify an Arrhenius reaction in CRANE, 
simply provide the Arrhenius expression for the rate coefficient
in the Chemical Reaction block of the file, for example: 

.. code-block:: toml 

    [ChemicalReactions]
      [./ScalarNetwork]
        species = 'e He He* O O_2 '
        equation_constants = 'Tg Te'
        equation_values = '345 2.5'
        equation_variables = 'T0 Teg'

        reactions = 'e + He* -> He + e         : { 7.0e-10 * (Teg)^(0.5) }
                     e + O_2 -> O + O + e      : { 1.41e-9 * Te^(0.22) * exp(-12.62/Te)}'
      [../]
    []