import pandas as pd
import numpy as np

###############
###############
def tokenize_string(reaction):
  reactant_list = []
  newstring = ''
  string_len = len(reaction)
  for i in range(string_len):
    if reaction[i] == '+' and reaction[i-1] != '{':
      reactant_list.append(newstring)
      newstring = ''
    elif reaction[i] == '-' and reaction[i-1] != '{':
      reactant_list.append(newstring)
      newstring = ''
    elif reaction[i] == '>' and reaction[i-1] == '-':
      reactant_list.append(u'->')
    #elif reaction[i] == '>':
    #  continue
    else:
      newstring = newstring + reaction[i]
  reactant_list.append(newstring)
  return reactant_list

###############
###############

def reactant_comparison(pls_include, reaction_list):
  reaction_list_length = len(reaction_list)-1
  reactant_counter = 0

  if reaction_list == [u'Reaction']:
    # stops 'Reaction' from being a valid reaction list
    reaction_list_length = 1e4

  for i in reaction_list:
    for j in pls_include:
      if i == j:
        reactant_counter = reactant_counter + 1
        break

  if reactant_counter == reaction_list_length:
    return_value = True
  else:
    return_value = False

  return return_value

###############
###############

def make_reaction_string(reaction_list):
  reaction_list_length = len(reaction_list)
  reaction_string = ''
  counter = 0
  bodies = -1
  for reactant in reaction_list:
    word_length = len(reactant)
    if reactant == '->':
      bodies = counter
      # reaction_string = reaction_string + '=>'
      reaction_string = reaction_string + ' -> '
      continue
    else:
      for i in range(word_length):
        if   reactant[i] == '^' and reactant[i+2] == '-':
          # reaction_string = reaction_string + '^-'
          reaction_string = reaction_string + '-'
        elif reactant[i] == '^' and reactant[i+2] == '+':
          # reaction_string = reaction_string + '^+'
          reaction_string = reaction_string + '+'
        elif reactant[i] == '^' and reactant[i+2] == '*':
          reaction_string = reaction_string + '*'
        elif reactant[i] == '{':
          continue
        elif reactant[i] == '}':
          continue
        elif reactant[i] == '+':
          continue
        elif reactant[i] == '-':
          continue
        elif reactant[i] == '*':
          continue
        elif reactant[i] == '_':
          continue
        else:
          reaction_string = reaction_string + reactant[i]

    if counter != reaction_list_length and reaction_list[counter+1] != '->':
      reaction_string = reaction_string + ' + '
    counter = counter + 1
  reaction_string = reaction_string[:-2]
  return reaction_string[:-1],bodies

###############
###############

def make_reaction_rate(reaction_rates,bodies):
  reaction_rate_list_length = len(reaction_rates)
  reaction_rate_string = ''
  counter = 0
  for rate in reaction_rates:
    if counter != 0:
      reaction_rate_string = reaction_rate_string + ' * '
    counter = counter + 1
    word_length = len(rate)
    for i in range(word_length):
      if rate[i] == 'x' and rate[i+1] == '1':                      # x10^{#}
        reaction_rate_string = reaction_rate_string + 'e'
      elif rate[i] == 'T' and rate[i+1] == '_' and rate[i+2] == 'e': #T_eg
        reaction_rate_string = reaction_rate_string + '(Teg)'
      elif rate[i] == 'T' and rate[i+1] == '_' and rate[i+2] == '0': #T_0
        reaction_rate_string = reaction_rate_string + '(T0)'
      elif rate[i] == 'T' and rate[i+1] == 'e':                      #Te
        reaction_rate_string = reaction_rate_string + 'Te'
      elif rate[i] == 'T' and rate[i+1] == 'g':                      #Tg
        reaction_rate_string = reaction_rate_string + 'Tg'
      elif rate[i] == '_':
        continue
      elif rate[i] == 'e' and rate[i+1] != 'x':
        continue
      elif rate[i] == 'g':
        continue
      elif rate[i] == '0' and rate[i-1] == '_':
        continue
      elif rate[i] == '0' and rate[i-2] == 'x':
        continue
      elif rate[i] == '1' and rate[i-1] == 'x':
        continue
      elif rate[i] == '^' and rate[i-3] == 'x':
        continue
      elif rate[i] == '^' and not(rate[i-3] == 'x'):
        # reaction_rate_string = reaction_rate_string + '**'
        reaction_rate_string = reaction_rate_string + '^'
      elif rate[i] == '{' and rate[i-4] == 'x':
        continue
      elif rate[i] == '}' and (rate[i-7] == 'x' or rate[i-8] == 'x'):
        continue
      elif rate[i] == '{' and not(rate[i-4] == 'x'):
        reaction_rate_string = reaction_rate_string + '('
      elif rate[i] == '}' and not(rate[i-7] == 'x' or rate[i-8] == 'x'):
        reaction_rate_string = reaction_rate_string + ')'
      else:
        reaction_rate_string = reaction_rate_string + rate[i]
#    if counter == 1 and bodies == 2:
#      reaction_rate_string = reaction_rate_string + '[cm^3 / s]'
#    elif counter == 1 and bodies == 3:
#      reaction_rate_string = reaction_rate_string + '[cm^6 / s]'
#    if bodies != 2 and bodies != 3:
#      print('Bodies = '+str(bodies))
#      print('Something happened')
#      exit()
#  if bodies == 2:
#    reaction_rate_string = reaction_rate_string + ' * N_A_const'
#  if bodies == 3:
#    reaction_rate_string = reaction_rate_string + ' * N_A_const^2'
  return reaction_rate_string

###############
###############


### Here we write the species we want to include
pls_include = ['e', 'He','He^{*}', 'He_2^{*}', 'He^{+}', 'He_2^{+}', 'O', 'O_2', 'O_3', 'O_2^{+}','O_2^{-}', 'O_4^{+}', 'M']
pls_include_change = ['e', 'He','He*', 'He_2*', 'He+', 'He_2+', 'O', 'O_2', 'O_3', 'O_2+','O_2-', 'O_4+', 'M']

#Sheet names to parse
included_sheet_names = ['A', 'B', 'C', 'D', 'E', 'G' ]



filename = 'reaction_list.xlsx'

xl = pd.ExcelFile(filename)


#print( sheet_a)


#print(column)
#print(column.isnull())
#print(column_isnan[2])

valid_reaction_number = 0
valid_reaction_array = []
valid_reaction_rates = []

for name in included_sheet_names:
  sheet_name = xl.parse(name)
  column = sheet_name['Unnamed: 2']
  reaction_rate_1 = sheet_name['Unnamed: 3']
  reaction_rate_2 = sheet_name['Unnamed: 4']
  reaction_rate_3 = sheet_name['Unnamed: 5']
  column_isnan = column.isnull()
  for i in range(column.size):
    if not(column_isnan[i]):
      reactant_list = tokenize_string(column[i])

      only_desired_reactants = reactant_comparison(pls_include, reactant_list)
      if only_desired_reactants:
        valid_reaction_number = valid_reaction_number + 1
        new_reaction_rate = []
        new_reaction_rate.append(reaction_rate_1[i])
        if type(reaction_rate_2[i]) != type(1.0) and type(reaction_rate_2[i]) != np.float64 :
          new_reaction_rate.append(reaction_rate_2[i])
        if type(reaction_rate_3[i]) != type(1.0) and type(reaction_rate_3[i]) != np.float64 :
          new_reaction_rate.append(reaction_rate_3[i])
        valid_reaction_array.append(reactant_list)
        valid_reaction_rates.append(new_reaction_rate)
        #print(column[i])


print('Valid reactions:   ',valid_reaction_number)
final_reactions = []
final_reaction_rates = []
for i in range(valid_reaction_number):
#  print i
  reaction_string, bodies = make_reaction_string(valid_reaction_array[i])
  final_reactions.append(reaction_string)
  final_reaction_rates.append(make_reaction_rate(valid_reaction_rates[i],bodies))
  if " * "  in final_reaction_rates[i]:
      final_reaction_rates[i] = '{'+final_reaction_rates[i]+'}'
  elif "^" in final_reaction_rates[i]:
      final_reaction_rates[i] = '{'+final_reaction_rates[i]+'}'
#  print(final_reactions[i])
#  print(final_reaction_rates[i])


fid = open('rate_file_crane.txt', 'w')
fid.write('Species list:\n')
fid.write('species = \'')
# for i in pls_include:
  # fid.write(i+' ')
for i in pls_include_change:
    fid.write(i+' ')
  # fid.write('\n')
fid.write('\'')
fid.write('\n\n')
fid.write('reactions = \'')
for i in range(valid_reaction_number):
  fid.write(final_reactions[i])
  # fid.write ('     ! R'+str(i))
  fid.write('     : '+str(final_reaction_rates[i]))
  if (i == valid_reaction_number-1):
      fid.write('\'\n')
  else:
      fid.write('\n')
fid.write('\n')
# for i in range(valid_reaction_number):
#   fid.write('$ R'+str(i)+' = ')
#   fid.write(final_reaction_rates[i])
#   fid.write('\n')
fid.close()


#for i in range(valid_reaction_number):
#  print(valid_reaction_rates[i])
#for i in range(valid_reaction_number):
#  print(make_reaction_string(valid_reaction_array[i]))


#print(valid_reaction_array[16])
#print(valid_reaction_rates[16])
#print(type(valid_reaction_rates[2][1]))
