import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

filename = 'nitrogen_out.csv'
data = pd.read_csv(filename)

fig, ax = plt.subplots()
time = data['time'] * 1e3
ax.semilogy(time, data['N'], 'k', label='N')
ax.semilogy(time, data['N2A'], 'r', label='N$_2$(A)')
ax.semilogy(time, data['N2C'], 'g', label='N$_2$(C)')
ax.semilogy(time, data['N2+'], 'b', label='N$_2^+$')
ax.semilogy(time, data['N4+'], 'c', label='N$_4^+$')
ax.set_xlabel('Time (ms)', fontsize=20)
ax.set_ylabel('Density (cm$^{-3}$)', fontsize=20)
ax.tick_params(axis='both', labelsize=15)
ax.legend(fontsize=10, loc='best', ncol=5)
ax.set_ylim([1e3, 1e14])
fig.savefig('nitrogen_species.png', bbox_inches='tight')
plt.show()
