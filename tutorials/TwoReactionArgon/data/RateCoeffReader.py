# import numpy for reading .txt file
import numpy as np

# read and store tabulated data using genfromtxt, ignoring lines with #
data = np.genfromtxt("lookUpTableRateCoeff.txt",comments='#')

# delete first line that contains non-acceptable string information
data = data[1:,:]

# select reduced electric field column
RedField = data[:,0] # Td

# select ionization rate coefficient
kIon = data[:,3] # m^3/s
# convert to cm3/s
kIon = kIon*1e6 # cm^3/s

# write CRANE-compatible lookup table
with open(r'ionization.txt','w') as f:
    np.savetxt(f,np.column_stack([RedField,kIon]),fmt='%.6e',delimiter=' ')

