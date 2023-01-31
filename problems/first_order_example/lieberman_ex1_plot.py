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

ka = 1
kb = 5

filename = 'lieberman_ex1_out_ka_'+str(ka)+'_kb_'+str(kb)+'.csv'
data = np.genfromtxt(filename, dtype=float, delimiter=',', skip_header=1)

t = data[:,0]
A = data[:,1]
B = data[:,2]
C = data[:,3]

t_exact = np.linspace(0, 3, 100)

# Crane solutions
plt.plot(t, A, linewidth=2, label='n$_A$')
plt.plot(t, B, linewidth=2, label='n$_B$')
plt.plot(t, C, linewidth=2, label='n$_C$')


plt.legend(loc='best', ncol=3)
plt.xticks(fontsize=15)
plt.yticks(fontsize=15)
plt.xlabel('t', fontsize=18)
plt.ylabel('n', fontsize=18)
plt.savefig('ex1_plot.png')
plt.show()
