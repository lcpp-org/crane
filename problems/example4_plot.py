#* This file is part of Crane, an open-source
#* application for plasma chemistry and thermochemistry
#* https://github.com/lcpp-org/crane
#*
#* Crane is powered by the MOOSE Framework
#* https://www.mooseframework.org
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html

import matplotlib.pylab as plt
from matplotlib.patches import Rectangle
import numpy as np
import csv

file = 'example4_out.csv'

data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
time = data[:,0]
N_N = data[:,1]
N_Ni = data[:,2]
N_N2 = data[:,3]
N_N2i = data[:,4]
N_N2A = data[:,5]
N_N2B = data[:,6]
N_N2C = data[:,7]
N_N3i = data[:,8]
N_N4i = data[:,10]
N_e = data[:,10]
EN = data[:,11]

fig = plt.figure(figsize=(8,8))
ax = plt.subplot(111)
plot11, = ax.semilogy(time, N_N, 'g-', label='N', alpha=0.7)
plot12, = ax.semilogy(time, N_N2A, 'b-', label='N$_2$(A)', alpha=0.7)
plot13, = ax.semilogy(time, N_N2C, 'r-', label='N$_2$(C)', alpha=0.7)
plot14, = ax.semilogy(time, N_N2i, 'c-', label='N$_2^+$', alpha=0.7)
plot15, = ax.semilogy(time, N_N4i, 'k-', label='N$_4^+$', alpha=0.7)
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.0), shadow=True, ncol=5)
plt.axis([0, 2.5e-3, 10**4, 10**15])
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Density [cm$^{-3}$]', fontsize=14)
plt.tick_params(axis='both', labelsize=11)
#plt.show()
plt.savefig('example4_plot.png')
plt.close()
