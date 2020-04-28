# Lorentz Attractor

!media media/examples/lorentz.png style=width:80%;margin-left:50px;float:center;id=fig:argon_example caption=Trajectory resulting from integrating the lorentz attractor equations.

The Lorentz Attractor is a simple set of three coupled nonlinear ODEs. 

\begin{equation}
\begin{aligned} 
\frac{dx}{dt} &= \sigma (y - x) \\
\frac{dy}{dt} &= x(p - z) - y \\
\frac{dz}{dt} &= xy - R z
\end{aligned}
\end{equation}

In this simulation, the parameters $\sigma$, p, and R are given values 10, 28, and 8/3, respectively. The `[ChemicalReactions]` block of the input file is shown below. They are added with brackets to denote that they are parsed functions; this allows the user to easily change the parameters from the input file. This system of "reactions" is equivalent to the system of ODEs. 

```

[ChemicalReactions]
  [./ScalarNetwork]
    species = 'x y z'
    equation_constants = 'sigma R p'
    equation_values = '10.0 8.0/3.0 28.0'

    reactions = 'x -> a                : {sigma}
                 y -> x + y            : {sigma}
                 x -> x + y            : {p}
                 y -> a                : 1.0
                 x + z -> x + y + z    : -1.0
                 x + y -> x + y + z    : 1.0
                 z -> a                : {R}'
  [../]
[]
```

