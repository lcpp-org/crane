import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import matplotlib.tri as tri



fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
file = 'example2_out.csv'
data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
y1 = data[:, 7]
y2 = data[:, 8]
y3 = data[:, 9]

ax.plot(y1, y2, y3, alpha=0.6)
plt.show()
