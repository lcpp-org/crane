import matplotlib.pylab as plt
import numpy as np
import csv

file = 'example5_out.csv'

data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
# time,N,N+,N2,N2+,N2A,N2B,N2C,N3+,N4+,e,reduced_field
time = data[:,0]
N_N = data[:,1]
N_Ni = data[:,2]
N_N2 = data[:,3]
N_N2i = data[:,4]
N_N2A = data[:,5]
N_N2B = data[:,6]
N_N2C = data[:,7]
N_N3i = data[:,8]
N_N4i = data[:,10]
N_e = data[:,10]
EN = data[:,11]


fig = plt.figure(figsize=(8,8))
ax = plt.subplot(111)
plot11, = ax.semilogy(time, N_N, 'g', label='$N$', alpha=0.7)
plot12, = ax.semilogy(time, N_N2A, 'b', label='$N2(A)$', alpha=0.7)
plot13, = ax.semilogy(time, N_N2C, 'r', label='$N2(C)$', alpha=0.7)
plot14, = ax.semilogy(time, N_N2i, 'c', label='$N_2^+$', alpha=0.7)
plot15, = ax.semilogy(time, N_N4i, 'k', label='$N_4^+$', alpha=0.7)
box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.1,
                 box.width, box.height * 0.9])
plt.legend()
plt.axis([0, 2.5e-3, 10**4, 10**14])
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Density [$cm^{-3}$]', fontsize=14)

plt.show()
