import math
import pandas as pd

m = 0.150 # mass [kg]
S = 1.5 # 1.5 - 2 safety factor
PU = 0.6 # suction force [bar]
n = 1 # number of suction cups

def diameter(m,S,PU,n):
    diameter = 1.12 * math.sqrt( (m * S) / (PU * n) ) * 10 # mm
    return diameter

PUs = [0.3, 0.4, 0.5, 0.6, 0.7]
ms = [0.150]*len(PUs)
ns = [1]*len(PUs)

diameters = [diameter(ms[i], S, PUs[i], ns[i]) for i in range(len(PUs))]

df = pd.DataFrame(list(zip(diameters, PUs)),
               columns =['Diameter (mm)', 'Suction Force (bar)'])

print('\n')
print(df)
print('\n')