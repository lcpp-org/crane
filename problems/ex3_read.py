from matplotlib.patches import Rectangle
import matplotlib.pylab as plt
import numpy as np
import csv
import collections
import pandas as pd
from sklearn.metrics import mean_squared_error

zdplaskin_file = '/Users/keniley/Documents/LCPP_Atmos/example3/out.dat'
# zdplaskin_rate_file = '/Users/keniley/Documents/LCPP_Atmos/example2/file_test.dat'

file = 'zdplaskin_ex3_csv_out.csv'
# rate_file = 'zdplaskin_ex2_csv_out_2.csv'

### READ REACTION RATES FROM ZDPLASKIN
# zd_rates_read = pd.read_csv(zdplaskin_rate_file, sep="  ")
# zd_rates = zd_rates_read.values
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
# rate_data = np.genfromtxt(rate_file, dtype=float, delimiter=',', skip_header=1)
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
N_N4i = data[:,9]
N_e = data[:,10]
EN = data[:,11]

# plt.plot(time, N_e)
# plt.show()
# exit()

# zdplaskin_data = np.genfromtxt(zdplaskin_file, delimiter='  ', skip_header=1)
zdplaskin_data = pd.read_csv(zdplaskin_file, sep="  ", header=0)
test = zdplaskin_data.values
zdstarttime = 0

zdtime = test[zdstarttime:,0]
zdne = test[zdstarttime:,2]
zdN = test[zdstarttime:,3]
zdNi = test[zdstarttime:,9]
zdN2 = test[zdstarttime:,4]
zdN2i = test[zdstarttime:,10]
zdN2A = test[zdstarttime:,5]
zdN2B = test[zdstarttime:,6]
zdN2C = test[zdstarttime:,8]
zdN3i = test[zdstarttime:,11]
zdN4i = test[zdstarttime:,12]

# plt.plot(time, N_e)
# plt.plot(zdtime, zdne)
# plt.show()
# exit()
#
# n = min(len(time), len(zdtime))
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
fig = plt.figure(figsize=(8,8))
ax = plt.subplot(111)
plot11, = ax.semilogy(time, N_N, 'g', label='$N$', alpha=0.7)
plot21, = ax.semilogy(zdtime, zdN, 'g--', label='$N$')

plot12, = ax.semilogy(time, N_N2A, 'b', label='$N2(A)$', alpha=0.7)
plot22, = ax.semilogy(zdtime, zdN2A, 'b--', label='$N2(A)$')

plot13, = ax.semilogy(time, N_N2C, 'r', label='$N2(C)$', alpha=0.7)
plot23, = ax.semilogy(zdtime, zdN2C, 'r--', label='$N2(C)$')

plot14, = ax.semilogy(time, N_N2i, 'c', label='$N_2^+$', alpha=0.7)
plot24, = ax.semilogy(zdtime, zdN2i, 'c--', label='$N_2^+$')

plot15, = ax.semilogy(time, N_N4i, 'k', label='$N_4^+$', alpha=0.7)
plot25, = ax.semilogy(zdtime, zdN4i, 'k--', label='$N_4^+$')
box = ax.get_position()
ax.set_position([box.x0, box.y0 + box.height * 0.1,
                 box.width, box.height * 0.9])
h, l = ax.get_legend_handles_labels()
ph = [plt.plot([], marker="", ls="")[0]]*2
handles = ph + h
labels = ["CRANE:", "ZDPlasKin:"] + l
# plt.legend(handles, labels, ncol=6, bbox_to_anchor=(0.015, -0.08))
plt.legend(handles, labels, ncol=6, bbox_to_anchor=(1.0, 1.12))
plt.axis([0, 2.5e-3, 10**4, 10**14])
plt.xlabel('Time [s]', fontsize=14)
plt.ylabel('Density [$cm^{-3}$]', fontsize=14)
# plt.savefig('ex3_comparison.png', bbox_inches='tight')
# plt.close()
# exit()

# plt.xlabel('Time [s]', fontsize=14)
# plt.ylabel('Density [$cm^{-3}$]', fontsize=14)
# plt.tick_params(axis='both', which='major', labelsize=12)
# plt.legend(ncol=2)
# title_proxy = Rectangle((0,0), -1, 0, color='w')
# plt.legend([title_proxy, plot11, plot12, plot13, plot14, plot15, title_proxy, plot21, plot22, plot23, plot24, plot25],
           # ["$CRANE$", "$N$", "$N2(A)$", "$N2(C)$", "$N_2^+$", "$N_4^+$", "$ZDPlasKin$", "$N$", "$N2(A)$", "$N2(C)$", "$N_2^+$", "$N_4^+$"], bbox_to_anchor=(1.4, 0.4), markerfirst=0, ncol=2, loc=4)
# plt.legend([title_proxy, plot12, plot13, plot14, plot15, title_proxy, plot22, plot23, plot24, plot25],
           # ["$CRANE$", "$Ar^+$", "$Ar^{2+}$", "$Ar^*$", "$e^-$", "$ZDPlasKin$", "$Ar^+$", "$Ar^{2+}$", "$Ar^*$", "$e^-$"], markerfirst=0, ncol=2, loc=4)
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
plt.show()
# plt.savefig('ex2_comparison2.png', bbox_inches='tight')
# plt.close()

# plt.loglog(time[crane_ind], N_Ar[crane_ind])
# plt.loglog(zdtime[zd_ind], zdAr[zd_ind])


# error = abs(N_Ar[crane_ind] - zdAr[zd_ind])/zdAr[zd_ind]
# plt.plot(time[crane_ind], error)
# plt.show()
