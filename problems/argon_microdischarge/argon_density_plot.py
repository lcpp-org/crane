import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

filename = 'argon_test_out.csv'
data = pd.read_csv(filename)

fig, ax = plt.subplots()
ax.loglog(data['time'], data['e'], 'k', label='e')
ax.loglog(data['time'], data['Ar+'], 'g', label='Ar$^+$')
ax.loglog(data['time'], data['Ar2+'], 'r', label='Ar$_2^+$')
ax.loglog(data['time'], data['Ar*'], 'c', label='Ar$^*$')
ax.set_xlabel('Time (s)', fontsize=20)
ax.set_ylabel('Density (cm$^{-3}$)', fontsize=20)
ax.tick_params(axis='both', labelsize=15)
ax.legend(fontsize=15)
fig.savefig('argon_species.png', bbox_inches='tight')
plt.show()
