import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt

type = ['stiff', 'nonstiff']
num = 5

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
for i in range(num):
    file = 'collisional_radiative_out_'+type[0]+'_'+'T'+str(i+1)+'.csv'
    data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
    y1 = data[:, 7]
    y2 = data[:, 8]
    y3 = data[:, 9]

    ax.plot(y1, y2, y3, alpha=0.6)

ax.set_xlabel('$H$')
ax.set_ylabel('$H^*$')
ax.set_zlabel('$H^+$')
ax.set_xlim3d(0, 1.0)
ax.set_ylim3d(0,1.0)
ax.set_zlim3d(0,1.0)
# ax.set_title('Nonstiff Trajectories')
ax.set_title('Stiff Trajectories')
ax.view_init(elev=10., azim=40)
ax.grid(False)
# Text
ax.text(1.0, 0, 0.05, "T1")  # (1, 0, 0)
ax.text(0.2, 0.05, 0.74, "T2") # (0.2, 0.05, 0.75)
ax.text(0.6, 0.2, 0.32, "T3") # (0.5, 0.2, 0.3)
ax.text(0.4, 0.4, 0.13, "T4") # (0.4, 0.4, 0.2)
ax.text(0.4, 0.6, 0.0, "T5") # (0.4, 0.6, 0.0)
# Nonstiff : 7.691916e-02 |   3.846604e-01 |   5.384204e-01
# Stiff: 6.618428e-10 |   6.978160e-02 |   9.302184e-01
# ax.text(7.69e-02, 3.85e-01, 5.38e-01, "Eq Point")
ax.text(6.618428e-10, 6.978160e-02, 9.302184e-01, "Eq Point")
# plt.show()
plt.savefig("stiff_transition.png", bbox_inches='tight')
plt.close()
