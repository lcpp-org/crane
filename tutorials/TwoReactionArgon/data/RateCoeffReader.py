import pandas as pd 
# import pandas for reading .txt file

# grabbing column names first
# convert to pandas data frame -> skiprows = row # with second set of ####
df = pd.read_csv("lookUpTableRateCoeff.txt",delim_whitespace=True,skiprows=10,engine='python')

# switching to cm3/s
df['R3_ine(m^3s^-1)'] = df['R3_ine(m^3s^-1)']*1e6

# write CRANE-compatible lookup table -> need to write such that it overwrites the text rather than appends to it
with open(r'ionization.txt', 'w') as f:
    dfAsString = df.to_string(header=False, index=False,columns=['RedField(Td)','R3_ine(m^3s^-1)'])
    f.write(dfAsString)


