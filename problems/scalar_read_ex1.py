import matplotlib.pylab as plt
import numpy as np
import csv
import collections
import pandas as pd

# zdplaskin_file = 'Ar2Reac_1000V_100kOhm_100t_0.4d_0.4r.dat'
zdplaskin_file = 'qt_densities.txt'
file = 'zdplaskin_ex1_scalar_csv_out.csv'

data = np.genfromtxt(file, dtype=None, delimiter=',', skip_header=1)
# zdplaskin_data = np.genfromtxt(zdplaskin_file, delimiter='  ', skip_header=1)
zdplaskin_data = pd.read_csv(zdplaskin_file, sep="  ", header=0)
test = zdplaskin_data.values
zdstarttime = 0
time = data[:,0]
N_Ar = data[:,1]
N_Ari = data[:,2]
N_e = data[:,3]


zdtime = test[zdstarttime:,0]
zde = test[zdstarttime:,1]
zdAr = test[zdstarttime:,2]
zdAri = test[zdstarttime:,3]

# plt.loglog(time, N_Ar, 'b', label='$Ar$')
# plt.loglog(time, N_Ari, 'g', label='$Ar^+$')
# plt.loglog(time, N_Ar2i, 'r', label='$Ar^{2+}$')
# plt.loglog(time, N_ArEx, 'c', label='$Ar^*$')
plt.semilogy(time, N_e, 'k', label='CRANE')
# plt.loglog(zdtime, zdAr, 'b--', label='$Ar$, ZDPlasKin')
# plt.loglog(zdtime, zdAri, 'g--', label='$Ar^+$, ZDPlasKin')
# plt.loglog(zdtime, zdAr2i, 'r--', label='$Ar^{2+}$, ZDPlasKin')
# plt.loglog(zdtime, zdArEx, 'c--', label='$Ar^*$, ZDPlasKin')
plt.semilogy(zdtime, zde, 'r--', label='ZDPlasKin')
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Electron Density [$cm^{-3}$]', fontsize=14)
plt.tick_params(axis='both', which='major', labelsize=12)
plt.legend(ncol=2)
# plt.show()
plt.savefig('ex1_comparision.png', bbox_inches='tight')
plt.close()
