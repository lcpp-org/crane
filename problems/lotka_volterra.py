import matplotlib.pylab as plt
import numpy as np

# file = 'lotka_volterra_out.csv'
file = 'test1_out.csv'

data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
time = data[:,0]
x = data[:,5]
y = data[:,6]
plt.plot(time, x, label='Species x', linewidth=2)
plt.plot(time, y, label='Species y', linewidth=2)
plt.legend()
plt.show()
# plt.savefig('lotka_volterra.png', bbox_inches='tight')
# plt.close()
