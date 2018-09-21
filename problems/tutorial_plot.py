import numpy as np
import matplotlib.pylab as plt

# Output CSV file name
file = 'action_scalar_test_csv_out.csv'


data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)

plt.plot(data[:,0], data[:,1])
plt.plot(data[:,0], data[:,2])
plt.show()
