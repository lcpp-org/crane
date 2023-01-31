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
import matplotlib.pylab as plt

# Output CSV file name
file = 'example1_out.csv'


data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)

plt.plot(data[:,0], data[:,5], 'k-', label='Species x')
plt.plot(data[:,0], data[:,6], 'b-', label='Species y')
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('`Species` count', fontsize=14)
plt.legend()
plt.show()
