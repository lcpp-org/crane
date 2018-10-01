import numpy as np
import matplotlib.pylab as plt

# Output CSV file name
file = 'zdplaskin_ex1_csv_out.csv'


data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)

plt.semilogy(data[:,0], data[:,2], 'ko-')
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Electron Density, [$cm^{-3}$]', fontsize=14)
plt.show()
# plt.savefig('example1.png', bbox_inches='tight')
# plt.close()
