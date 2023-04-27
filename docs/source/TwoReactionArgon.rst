Simple Two-Reaction Argon Plasma
================================

Problem Description
-------------------

Here, we consider a simple argon plasma composed of electrons, atomic ions, and the background gas.
The source of ions and electrons is electron-impact ionization, and the sink is three-body recombination:

.. math::
    :label: two_reaction_mechanism

    \text{e} + \text{Ar} \to \text{e} + \text{e} + \text{Ar}^{+} 

    \text{e} + \text{Ar}^{+} + \text{Ar} \to \text{Ar} + \text{Ar} 

The electron Boltzmann equation solver `LoKI-B <https://github.com/IST-Lisbon/LoKI>` is used to solve for the rate coefficient of ionization. 
The recombination rate coefficient is assumed to be constant at :math:`k = 10^{-25} \; \text{cm}^6/\text{s}`.

Theory
------

For each our three species under consideration, we can write an ordinary differential equation that describes the change in density due to the chemical reactions:

.. math::
    :label: ODE_system

    \frac{d n_e}{d t} = k_i n_e n_{Ar} - k_r n_e n_i n_{Ar},

    \frac{d n_i}{d t} = k_i n_e n_{Ar} - k_r n_e n_i n_{Ar},
    
    \frac{d n_{Ar}}{d t} = - k_i n_e n_{Ar} + k_r n_e n_i n_{Ar},

where :math:`n_e`, :math:`n_i`, :math:`n_{Ar}` are the number densities (:math:`\text{cm}^{-3}`) of the electrons, ions, and neutral gas, respectively, 
and :math:`k_i` (:math:`\text{cm}^3/\text{s}`) and :math:`k_r` (:math:`\text{cm}^6/\text{s}`) are the rate coefficients of ionization and recombination.



