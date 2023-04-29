import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# read .csv output using pandas
filename = 'TwoReactionArgon_out.csv'
data = pd.read_csv(filename)

# use rate coefficients to determine steady-state value
ion_coeff = data.loc[3].at["rate_constant0"] #cm3/s at 30 Td
recomb_coeff = data.loc[3].at["rate_constant1"] #cm6/s

ss_n = ion_coeff/recomb_coeff # steady-state prediction cm^-3

fig = plt.figure()
plt.axhline(y=ss_n,color='blue',linestyle='--',label="Steady-State Prediction")
plt.plot(data['time'], data['e'], color='red', label='Computation Result')
plt.yscale("log")
plt.xscale("log")
plt.xlabel("Time (s)")
plt.ylabel("Plasma Density [cm${}^{-3}$]")
plt.legend()
plt.grid(linestyle='--',alpha=0.9)
plt.draw()
plt.savefig("plasma_density.png",dpi=600)

