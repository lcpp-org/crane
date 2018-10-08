import numpy as np
import matplotlib.pylab as plt

# Output CSV file name
file = 'zdplaskin_ex1_out.csv'
file2 = 'example1_log_out.csv'
zdplaskin_file = '/Users/keniley/Documents/LCPP_Atmos/example1/out.txt'

# zdplaskin_data = pd.read_csv(zdplaskin_file, sep="  ", header=0)
zd_data = np.genfromtxt(zdplaskin_file, dtype=float, delimiter="  ", skip_header=14, skip_footer=2)
# test = zdplaskin_data.values

data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
data2 = np.genfromtxt(file2, dtype=float, delimiter=',', skip_header=1)

plt.semilogy(data[:,0], data[:,3], 'ko-')
plt.semilogy(data2[:,0], np.exp(data2[:,3]), 'ro-')
# plt.semilogy(data2[:,0], data2[:,4], 'ro-')
# plt.semilogy(zd_data[:,0], zd_data[:,1], 'go-')
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Electron Density, [$cm^{-3}$]', fontsize=14)
plt.show()
# plt.savefig('example1.png', bbox_inches='tight')
# plt.close()
