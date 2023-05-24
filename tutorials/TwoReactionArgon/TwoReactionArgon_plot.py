 # import numpy for reading .txt file
import numpy as np 
# import matplotlib for visualization
import matplotlib.pyplot as plt

# read and store tabulated data using genfromtxt, ignoring first line with strings
data = np.genfromtxt("TwoReactionArgon_out.csv",skip_header=1,delimiter=',')

'''
0 : time
Variables come in alphabetical order
1 : Ar
2 : Ar+
3 : e
Rate coefficients come after variables, in order of introduction
4 : k_ion
5 : k_rec
'''

# select time column
time = data[:,0] # s

# select electron density column
n_e = data[:,3] # cm^-3

# select ionization rate coefficient
k_ion = data[:,4] # cm^3 s^-1 

# select recombination rate coefficient
k_rec = data[:,5] # cm^6 s^-1

# both are constant, grab last timestep, divide to get steady-state density
ss_n = k_ion[-1]/k_rec[-1] # cm^-3

# early-time prediction
def rise(t):
    n_p0 = 1 # cm^-3
    n_Ar = 2.5e19 # cm^-3
    return n_p0*np.exp(n_Ar*k_ion*t)

fig = plt.figure()
plt.axhline(y=ss_n,color='blue',linestyle='--',label="Steady-State Prediction")
plt.plot(time,rise(time),color='green',linestyle='--',label="Initial Growth Prediction")
plt.plot(time, n_e, color='red', label='Computation Result')
plt.yscale("log")
plt.xscale("linear")
plt.xlabel("Time (s)")
plt.ylabel("Plasma Density [cm${}^{-3}$]")
plt.text(2e-7, 1e4, r'slope: $k_{ion}n_{Ar}$', fontsize=12)
plt.text(3.5e-7, 1e14, r'value: $k_{ion}/k_{rec}$', fontsize=12)
plt.legend()
plt.grid(linestyle='--',alpha=0.9)
plt.draw()
plt.savefig("TwoReactionArgon_density.png",dpi=600)