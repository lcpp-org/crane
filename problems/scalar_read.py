from matplotlib.patches import Rectangle
import matplotlib.pylab as plt
import numpy as np
import csv
import collections
import pandas as pd
from sklearn.metrics import mean_squared_error

zdplaskin_file = '/Users/keniley/Documents/LCPP_Atmos/example2/Ar2Reac_1000V_100kOhm_100t_0.4d_0.4r.dat'
zdplaskin_rate_file = '/Users/keniley/Documents/LCPP_Atmos/example2/file_test.dat'
# file = 'zdplaskin_ex2_scalar_csv_out_newton.csv'
# file = 'zdplaskin_ex2_scalar_csv_out.csv'
file = 'parsed_test_csv_out.csv'
rate_file = 'parsed_test_csv_out_2.csv'


### READ REACTION RATES FROM ZDPLASKIN
zd_rates_read = pd.read_csv(zdplaskin_rate_file, sep="  ")
zd_rates = zd_rates_read.values
# with open(zdplaskin_file, 'rU') as infile:
#   # read the file as a dictionary for each row ({header : value})
#   reader = csv.DictReader(infile)
#   data = {}
#   for row in reader:
#     for header, value in row.items():
#       try:
#         data[header].append(value)
#       except KeyError:
#         data[header] = [value]
#
#
# print(data["time"])
# collections.OrderedDict(sorted(data.items(), key=lambda t: t[0]))

# extract the variables you want
# time = data['time']
# N_Ar = data['Ar']
# N_Ari = data['Ar+']
# N_Ar2i = data['Ar2+']
# N_ArEx = data['Ar*']
# N_e = data['e']
#
# plt.plot(time)
# plt.show()

# lines = open(file).readlines()
# print(lines)
# exit()

data = np.genfromtxt(file, dtype=float, delimiter=',', skip_header=1)
rate_data = np.genfromtxt(rate_file, dtype=float, delimiter=',', skip_header=1)

time = data[:,0]
N_Ar = data[:,1]
N_Ari = data[:,3]
N_Ar2i = data[:,4]
N_ArEx = data[:,2]
N_e = data[:,5]
EN = data[:,7]
mu = data[:,6]

# time = data[:,0]
# N_Ar = data[:,1]
# N_Ari = data[:,3]
# N_Ar2i = data[:,4]
# N_ArEx = data[:,2]
# N_e = data[:,5]
# EN = data[:,6]

# zdplaskin_data = np.genfromtxt(zdplaskin_file, delimiter='  ', skip_header=1)
zdplaskin_data = pd.read_csv(zdplaskin_file, sep="  ", header=0)
test = zdplaskin_data.values
zdstarttime = 1

zdtime = test[zdstarttime:,0]
zdAr = test[zdstarttime:,4]
zdAri = test[zdstarttime:,6]
zdAr2i = test[zdstarttime:,7]
zdArEx = test[zdstarttime:,5]
zde = test[zdstarttime:,3]
zdefield = test[zdstarttime:,2]
zdmu = test[zdstarttime:,7]

n = min(len(time), len(zdtime))
# out_idx = np.flatnonzero(time[:n] == zdtime[:n])
# out_val = time[out_idx]


# count = 0
# crane_ind = []
# zd_ind = []
# for i in range(len(time)):
#     diff_min = time[i] - time[i]*0.001
#     diff_max = time[i] + time[i]*0.001
#     for j in range(len(zdtime)):
#         if (diff_min <= zdtime[j] <= diff_max ):
#             # count = count + 1
#             crane_ind.append(i)
#             zd_ind.append(j)
#             continue

# print(len(N_Ar[crane_ind]))
# print(len(zdAr[zd_ind]))
# print(mean_squared_error(N_Ar[crane_ind], zdAr[zd_ind]))
# exit()
# plot00, = plt.semilogx(time, EN*1e21)
# plot11, = plt.loglog(time, N_Ar, 'b', label='$Ar$, CRANE')
plot12, = plt.loglog(time, N_Ari, 'g', label='$Ar^+$, CRANE')
plot13, = plt.loglog(time, N_Ar2i, 'r', label='$Ar^{2+}$, CRANE')
plot14, = plt.loglog(time, N_ArEx, 'c', label='$Ar^*$, CRANE')
plot15, = plt.loglog(time, N_e, 'k', label='$e^-$, CRANE')
# plot21, = plt.loglog(zdtime, zdAr, 'b--', label='$Ar$, ZDPlasKin')
plot22, = plt.loglog(zdtime, zdAri, 'g--', label='$Ar^+$, ZDPlasKin')
plot23, = plt.loglog(zdtime, zdAr2i, 'r--', label='$Ar^{2+}$, ZDPlasKin')
plot24, = plt.loglog(zdtime, zdArEx, 'c--', label='$Ar^*$, ZDPlasKin')
plot25, = plt.loglog(zdtime, zde, 'k--', label='$e^-$, ZDPlasKin')
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Density [$cm^{-3}$]', fontsize=14)
# plt.tick_params(axis='both', which='major', labelsize=12)
# plt.legend(ncol=2)
title_proxy = Rectangle((0,0), -1, 0, color='w')
# plt.legend([title_proxy, plot11, plot12, plot13, plot14, plot15, title_proxy, plot21, plot22, plot23, plot24, plot25],
#            ["$CRANE$", "Ar", "$Ar^+$", "$Ar^{2+}$", "$Ar^*$", "$e^-$", "$ZDPlasKin$", "Ar", "$Ar^+$", "$Ar^{2+}$", "$Ar^*$", "$e^-$"], markerfirst=0, ncol=2, loc=4)
plt.legend([title_proxy, plot12, plot13, plot14, plot15, title_proxy, plot22, plot23, plot24, plot25],
           ["$CRANE$", "$Ar^+$", "$Ar^{2+}$", "$Ar^*$", "$e^-$", "$ZDPlasKin$", "$Ar^+$", "$Ar^{2+}$", "$Ar^*$", "$e^-$"], markerfirst=0, ncol=2, loc=4)
# plt.semilogx(time, EN*1e21)
# plt.semilogx(zdtime, zdefield)
# plt.legend()
# plt.plot(time, mu)
# plt.plot(zdtime, zdmu*1e-4)

#
# fig, ax1 = plt.subplots()
# ax1.loglog(time, N_e, 'k')
# ax1.loglog(zdtime, zde, 'k--')
# ax1.set_ylabel('Electron Density [cm^3]')
#
# ax2 = ax1.twinx()
# ax2.semilogx(time, EN*1e21, 'r')
# ax2.semilogx(zdtime, zdefield, 'r--')
# ax2.set_ylabel('Reduced Field [Td]')

# ax1.loglog(rate_data[1:, 0], rate_data[1:, 3], 'k', label='CRANE')
# ax1.loglog(zd_rates[:, 0], zd_rates[:, 3], 'r--', label='ZDPlasKin')
# ax1.set_ylabel('Rate Coefficients')

# ax2 = ax1.twinx()
# ax2.semilogx(time, EN*1e21, 'r', alpha=0.5)
# ax2.semilogx(zdtime, zdefield, 'r--', alpha=0.5)
# ax2.set_ylabel('Reduced Field [Td]')
# plt.show()
# plt.savefig('ex2_true_comparison.png', bbox_inches='tight')
# plt.close()

# plt.loglog(time[crane_ind], N_Ar[crane_ind])
# plt.loglog(zdtime[zd_ind], zdAr[zd_ind])


# error = abs(N_Ar[crane_ind] - zdAr[zd_ind])/zdAr[zd_ind]
# plt.plot(time[crane_ind], error)
plt.show()
