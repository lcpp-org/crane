import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('dc_crane_test_output.csv', dtype='float', delimiter=',',skiprows=1)

x = data[:,0]
nar = data[:,1]
ne = data[:,2]

plt.plot(x,ne, 'r', linewidth=2, label='$n_e$')
plt.plot(x,nar, 'k', linewidth=2, label='$n_{Ar^+}$')
plt.xlabel("X [m]", fontsize=12)
plt.ylabel("Density [$m^{-3}$]", fontsize=12)
plt.legend()
# plt.show()
plt.savefig('dc_crane_test.png', bbox_inches='tight')
plt.close()
