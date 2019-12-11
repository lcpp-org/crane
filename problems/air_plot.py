import numpy as np
import matplotlib.pylab as plt
import csv
import pandas as pd

# Output CSV file name
#file = 'air_discharge_mod_out.csv'
file = 'air_voltage_out.csv'
file2 = 'air_discharge_mod_out_temp.csv'

species = ['N','N+','N2','N2+','N2A3','N2B3','N2C3','N2D','N2P','N2a_1','N2v1','N2v2','N2v3','N2v4','N2v5','N2v6','N2v7','N2v8','N3+','N4+','NO','NO+','NO-','O','O+','O-','O1D','O1S','O2','O2+','O2-','O24_ev','O2a1','O2b1','O2v1','O2v2','O2v3','O2v4','O3','O3-','O4+','O4-','O2pN2', 'NEUTRAL']

plot_species = ['O3','O','NO','O2a1','N2A3','N','O2b1','O1S', 'NEUTRAL']
#plot_species = np.copy(species)
#plot_species = ['reduced_field']
#plot_species = ['voltage']


test = pd.read_csv(file)
test2 = pd.read_csv(file2)
#plt.plot(test['time'], test['O3'], label='O3')
#plt.plot(test['time'], test['NO'], label='NO')
#plt.plot(test['time'], test['O2a1'], label='O2a1')
#plt.plot(test['time'], test['O'], label='O')
#plt.plot(test['time'], test['N2A3'], label='N2A3')
#plt.plot(test['time'], test['O1S'], label='O1S')
#plt.plot(test['time'], test['O2b1'], label='O2b1')
#plt.plot(test['time'], test['N'], label='N')
#plt.plot(test['time'], test['O2'], label='O$_2$')
#plt.plot(test['time'], test[''], label='comp')
#plt.loglog(test['time'], test['e'], label='O2(v2)')

NUM_COLORS = len(plot_species)
#LINE_STYLES = ['solid', 'dashed', 'dashdot', 'dotted']
LINE_STYLES = ['solid', 'dashdot']
LINE_STYLES2 = ['dashed', 'dotted']
NUM_STYLES = len(LINE_STYLES)
cm = plt.get_cmap('brg')
fig = plt.figure(figsize=(10,10))
ax = fig.add_subplot(111)
for i in range(len(plot_species)):
    lines = ax.semilogy(test['time'], test[plot_species[i]], label=plot_species[i])
    lines[0].set_color(cm(i//NUM_STYLES*float(NUM_STYLES)/NUM_COLORS))
    lines[0].set_linestyle(LINE_STYLES[i%NUM_STYLES])
    #lines2 = ax.semilogy(test2['time'], test2[plot_species[i]]) 
    #lines2[0].set_color(cm(i//NUM_STYLES*float(NUM_STYLES)/NUM_COLORS))
    #lines2[0].set_linestyle(LINE_STYLES2[i%NUM_STYLES])
plt.legend(ncol=3)
#plt.show()
plt.savefig('air_plot_transient.png', dpi=200, bbox_inches='tight')
plt.close()
exit()


exit()

