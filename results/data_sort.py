import pandas as pd
import numpy as np
bc500 = pd.read_csv("OpenFlightsBC500.txt",header=None,sep=" ")
bc500.rename(columns={0:"Airport",1:"BCVal"}, inplace=True)
bc500.sort_values("BCVal", ascending=False, inplace=True)
np.savetxt("Sorted500BC.txt", bc500.to_numpy(), fmt = "%s")
bc1000 = pd.read_csv("OpenFlightsBC1000.txt",header=None,sep=" ")
bc1000.rename(columns={0:"Airport",1:"BCVal"}, inplace=True)
bc1000.sort_values("BCVal", ascending=False, inplace=True)
np.savetxt("Sorted1000BC.txt", bc1000.to_numpy(), fmt = "%s")
