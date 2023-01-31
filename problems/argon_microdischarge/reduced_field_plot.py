#* This file is part of Crane, an open-source
#* application for plasma chemistry and thermochemistry
#* https://github.com/lcpp-org/crane
#*
#* Crane is powered by the MOOSE Framework
#* https://www.mooseframework.org
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

filename = 'argon_test_out.csv'
data = pd.read_csv(filename)

fig, ax = plt.subplots()

ax.semilogx(data['time'], data['reduced_field'], 'k', label='E/N', linewidth=4)

ax.set_xlabel('Time (s)', fontsize=20)
ax.set_ylabel('Reduced Field, E/N (V m$^{-2}$)', fontsize=20)
ax.tick_params(axis='both', labelsize=15)
ax.legend(fontsize=15)
fig.savefig('reduced_field.png', bbox_inches='tight')
plt.show()
