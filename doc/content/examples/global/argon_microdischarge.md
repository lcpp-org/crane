# Argon Microdischarge

This example is based on a [ZDPlasKin example of an argon microdischarge at atmospheric pressure](http://www.zdplaskin.laplace.univ-tlse.fr/index.html@p=310.html) [!citep](zdplaskin2008). The model includes five species: Ar, Ar$^*$, Ar$^+$, Ar$_2^+$, and electrons. The number density versus time is shown in the plot below, with ZDPlasKin's results for comparison.

The reactions are included in the input file as shown below:

```
[ChemicalReactions]
  [./ScalarNetwork]
    species = 'e Ar* Ar+ Ar Ar2+'
    reaction_coefficient_format = 'rate'
    file_location = 'Example3'

    # These are parameters required equation-based rate coefficients
    equation_constants = 'Tgas J pi'
    equation_values = '300 2.405 3.141'
    equation_variables = 'Te'
    rate_provider_var = 'reduced_field'
    sampling_variable = 'reduced_field'


    reactions = 'e + Ar -> e + e + Ar+          : EEDF
                 e + Ar -> Ar* + e              : EEDF
                 e + Ar* -> Ar + e              : EEDF
                 e + Ar* -> Ar+ + e + e         : EEDF
                 Ar2+ + e -> Ar* + Ar           : {8.5e-7*((Te/1.5)*11600/300.0)^(-0.67)}
                 Ar2+ + Ar -> Ar+ + Ar + Ar     : {(6.06e-6/Tgas)*exp(-15130.0/Tgas)}
                 Ar* + Ar* -> Ar2+ + e          : 6.0e-10
                 Ar+ + e + e -> Ar + e          : {8.75e-27*((Te/1.5)^(-4.5))}
                 Ar* + Ar + Ar -> Ar + Ar + Ar  : 1.399e-32
                 Ar+ + Ar + Ar -> Ar2+ + Ar     : {2.25e-31*(Tgas/300.0)^(-0.4)}
                 e -> W                         : {1.52*(760/100)*(Tgas/273.16)*(Te/1.5)*((J/0.4)^2 + (pi/0.4)^2)}
                 Ar+ -> W                       : {1.52*(760/100)*(Tgas/273.16)*(Te/1.5)*((J/0.4)^2 + (pi/0.4)^2)}
                 Ar2+ -> W                      : {1.52*(760/100)*(Tgas/273.16)*(Te/1.5)*((J/0.4)^2 + (pi/0.4)^2)}'
  [../]
[]
```

Rate coefficients are separated from the reaction notation by a colon character. If a rate coefficient is enclosed in brackets, it will be added as a parsed function. (Note that this may incur a slight performance penalty, but the benefit is that any functional form may be included.) Variables that are added as constants inside the function must be noted with the `equation_values` parameter, and the values assigned with `equation_values`. Any nonlinear or auxiliary variables must be specified with the `equation_constants` parameter.

!media media/examples/argon.png style=width:80%;margin-left:50px;float:center;id=fig:argon_example caption=Argon species and electrons versus time. Solid lines are computed by CRANE, dotted lines by ZDPlasKin.

!alert note title=Plot Differences
The profiles are not identical. The main difference between ZDPlasKin and CRANE is that a Boltzmann solver is not dynamically coupled to CRANE, but [BOLSIG+](http://www.bolsig.laplace.univ-tlse.fr/) is coupled and run with ZDPlasKin. Instead, electron-impact rate coefficients are tabulated prior to runtime.

!bibtex bibliography
