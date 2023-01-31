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
import numpy as np
import csv

file = 'example3_out.csv'

data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)

time = data[:,0]
N_Ar = data[:,1]
N_Ari = data[:,3]
N_Ar2i = data[:,4]
N_ArEx = data[:,2]
N_e = data[:,7]

plot12, = plt.loglog(time, N_Ari, 'g', label='$Ar^+$')
plot13, = plt.loglog(time, N_Ar2i, 'r', label='$Ar^{2+}$')
plot14, = plt.loglog(time, N_ArEx, 'c', label='$Ar^*$')
plot15, = plt.loglog(time, N_e, 'k', label='$e^-$')
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Density [$cm^{-3}$]', fontsize=14)
plt.legend()
plt.show()
