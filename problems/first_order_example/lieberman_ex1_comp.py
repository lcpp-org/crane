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

def exact_solution(time, n0, KA, KB):
    '''
    From textbook Lieberman, "Principles of Plasma Discharges and Materials Processing"
    Chapter 9.2, page 290 (second edition)
    "First-Order Consecutive Reactions"
    Equations 9.2.5 (nA), 9.2.6 (nB), 9.2.8 (nC)
    '''
    nA = n0 * np.exp(-KA * time)
    
    nB = n0 * KA / (KB - KA) * (np.exp(-KA * time) - np.exp(-KB * time))

    nC = n0 * (1. + 1./(KA - KB) * (KB * np.exp(-KA * time) - KA * np.exp(-KB * time)))

    return(nA, nB, nC)

n0 = 1
ka = 1
kb = 5

filename = 'lieberman_ex1_out_ka_'+str(ka)+'_kb_'+str(kb)+'.csv'
data = np.genfromtxt(filename, dtype=float, delimiter=',', skip_header=1)

t = data[:,0]
A = data[:,1]
B = data[:,2]
C = data[:,3]

t_exact = np.linspace(0, 3, 100)
A_exact, B_exact, C_exact = exact_solution(t_exact, n0, ka, kb)

# This just gets the default color cycle so I can plot the Crane solutions
# against the exact solutions using the same colors
color_list = plt.rcParams['axes.prop_cycle'].by_key()['color']

# Crane solutions
plt.plot(t, A, color=color_list[0], linewidth=2, label='n$_A$')
plt.plot(t, B, color=color_list[1], linewidth=2, label='n$_B$')
plt.plot(t, C, color=color_list[2], linewidth=2, label='n$_C$')


plt.legend(loc='best', ncol=3)
plt.xticks(fontsize=15)
plt.yticks(fontsize=15)
plt.xlabel('t', fontsize=18)
plt.ylabel('n', fontsize=18)

# Exact solutions
plt.plot(t_exact, A_exact, '--.', color=color_list[0], linewidth=2, label='A (exact)')
plt.plot(t_exact, B_exact, '--.', color=color_list[1], linewidth=2, label='B (exact)')
plt.plot(t_exact, C_exact, '--.', color=color_list[2], linewidth=2, label='C (exact)')
plt.savefig('ex1_comp_plot.png')
plt.show()
