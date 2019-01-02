import pandas as pd
INPUT = 'process.csv'
r = pd.read_csv(INPUT)
params = []
for i in range(8):
    params.append(r.iloc[:, i + 5].tolist())

# matplotlib
import matplotlib.pyplot as plt
import numpy as np

x = np.arange(10) 

x = [i for i in range(len(params[0]))]
legend_list = ['table', 'corner', 'counting', 'action', 'front', 'close-corner', 'border', 'halfstable']

for i in range(len(legend_list)):
    # if i == 1 or i == 5: continue
    plt.plot(x, params[i], label=legend_list[i])
plt.legend(loc=2)
plt.show()

# colormap = plt.cm.gist_ncar 
# colors = [colormap(i) for i in np.linspace(0, 1,len(ax1.lines))]
# for i,j in enumerate(ax1.lines):
#     j.set_color(colors[i])


# plt.savefig('smooth_plot.png')
