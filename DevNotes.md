# 2018
## Oct. 8
1. Updated CRANE repo. Included a few modifications to allow for direct coupling with Zapdos.
2. Zapdos: created new BC in Zapdos based on HagelaarEnergyBC, called EnergyBC2 (tentative name). Accepts arbitrary number of ionized species. However, cannot accept an arbitrary number of material properties. Can we declare a vector of Material properties in MOOSE?
3. Started development of gas temperature tracking in CRANE. Should be relatively straightforward - the format is essentially the same as the regular kernels, but with the energy change multiplied in. The energy change is already parsed and included by ChemicalReactionsBase. Should be the same format as the energy kernels implemented in Zapdos, but with the signs flipped.
4. Integrating Bolsig+ is still a big fat question mark. Even if it works, it would probably slow down simulations considerably. And how can an external 0D MultiApp be handled in parallel simulations? Tricky. It might actually be better to include a MOOSE Boltzmann solver...


## Oct. 9
1. Created new kernels for gas temperature calculation: EnergyTermScalar and ODETimeDerivativeTemperature. Still requires:
  * Coupled gas density as a variable (ODETimeDerivativeTemperature) - if n_gas includes multiple species this should be either an AuxVariable (SpeciesSum) or a vector of coupled variables. The latter would make it infinitely easier to calculate the off-diag jacobian, but the former is more modular.
  * Variable-dependent rate constants (EnergyTermScalar). This can be complicated, and I'm not sure how to automate it for all possible cases.
2. REALLY need to swap out the "n_gas" parameter. It should be an AuxVariable, not a <Real> input parameter. Future work...
3. After a few bug fixes in the Network sub-block, it looks like CRANE has no trouble running "scalar" problems utilizing spatial variables and kernels (dummy grid with nx = 1). This is good! I can make sure both ScalarNetwork and Network are properly running with the same tests now.
  * However, this also begs the question: is the scalar version even necessary? It's much harder to automate derivative terms without Materials.
  * Apparently it is still useful! Running 'zdplaskin_ex2.i' - runtime = 1.109 s; Running 'kernel_test.i' - runtime = 3.859 s. That's a ~3.5x speedup just by using scalar variables. Might still be very useful for large 0D problems. (3.859 s -> 3.149 s using linear solver instead of newton)

## Oct. 10
1. Implemented multi-ion capabilities in Zapdos (utilizing the CRANE reaction network!).
  * Needed to modify mean_en boundary conditions (new BC: EnergyBC2) to accept a vector of ions and vectors of material properties (for ion mobility, diffusivity, and charge).
  * Test case: sakiyama_rf_1D.i.  It works...but JouleHeating just continuously increases electron temperature to the point of absurdity. (50 eV? Really?)
  * Probable cause: incorrect electron mobility and diffusion coefficients (from BOLSIG+). Still using data for an Ar discharge, not He/N. Unfortunately, BOLSIG+ can only be a rough estimate at the moment. Not dynamically coupled yet. Hopefully the species concentrations do not change w.r.t. the gas densities too much.

## Oct. 11
1. Big issue - EEDF reactions in the form of rates (not townsend formulation) do not contribute to the residual. They do if they use the townsend formulation since it uses the Zapdos kernels.
  * How do I check to see if the rate coefficient changes with a variable? It might not be possible to keep it COMPLETELY general and automatic, unfortunately.
  * For now, keep EEDF values to Townsend formulation. Add modification to include actual_mean_en sampling derivatives. Any other sampler just...will not work.
  * LONG TERM: How can this possibly be reconciled with a MOOSE-based BOLSIG+ solver? It will still have the variable dependence, but we will no longer have access to sampleDerivative. Ugh. That is complicated.
2. (Oct. 12 update): I think this was fixed! Kernels are added properly.

## Oct. 12
1. Lots of issues with the Sakiyama test case (sakiyama_rf_1D.i).
  * Solution is STRONGLY dependent on the offset parameter in LogStabilizationMoles_kV. Not good.
2. Added kernels to Zapdos to handle elastic energy changes (no "energy_elastic" material now - just calculated within the kernel). Still need to update the other kernels and materials to accept this change...
3. Streamline-upwind petrov-galerkin - replace log form while still ensuring positivity (better convergence than log density format)
  * Possible to do before APS (Nov 5)? Probably not. Should be done this year though. Log form feels so cumbersome. It only serves to complicate the energy-dependent Jacobians, which are already complicated to begin with!
4. !!!!! Aux variables coupling is broken! Probably has to do with the jacobian terms.
