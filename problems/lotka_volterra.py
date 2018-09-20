import matplotlib.pylab as plt
import numpy as np
import csv
import collections

file = 'action_scalar_test_csv_out.csv'

# with open(file, 'rU') as infile:
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

data = np.genfromtxt(file, dtype=None, delimiter=',', skip_header=1)
time = data[:,0]
x = data[:,1]
y = data[:,2]

plt.plot(time, x)
plt.plot(time, y)
plt.show()
