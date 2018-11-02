import numpy as np
import matplotlib.pylab as plt

# Output CSV file name
file = 'example1_out.csv'


data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)

plt.(data[:,0], data[:,5], 'ko-')
plt.plot
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Electron Density, [$cm^{-3}$]', fontsize=14)
plt.show()
