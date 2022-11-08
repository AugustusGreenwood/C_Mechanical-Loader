import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
from matplotlib.ticker import AutoMinorLocator
import os
import re

mpl.rcParams['axes.linewidth'] = 2

'''
def setAxisParameters(ax):
    ax.tick_params('both', direction='in', which='both', length=11, width=2, labelsize=17, pad=7)
    ax.tick_params('both', which='minor', length=7, width=1.5)

    ax.xaxis.set_minor_locator(AutoMinorLocator(2))
    ax.yaxis.set_minor_locator(AutoMinorLocator(2))

    ax.set_title("300ms acceleration", fontsize=24, pad=20)


    ax.set_xlabel("Time (s)", fontsize=20)
    ax.set_ylabel("Pulses", fontsize=20)

    ax.set_xlim(0, 7)
    ax.set_ylim(-500, 0)

    ax.legend(title="High speed (pulses/sec)", bbox_to_anchor=(0.725, 1))

    #ax.grid(which='major', axis='x', linewidth=2, color='k', alpha=0.5, linestyle='--')


path = '/home/augustus/Documents/Mechanical-Loader/python_data/DW-0_AC-300_0.5mm_SIN_varHS/'

files = [file for file in os.listdir(path) if file[-3::] == "txt"]

hs = [re.search("DW-0_HS-(.+?)_AC-300_0.5mm_SIN.txt", file).group(1) for file in files]
print(hs)
wanted_files = []
wanted_hs = []
wanted_files.extend([files[5], files[0], files[1]])
wanted_hs.extend([hs[5], hs[0], hs[1]])


data = [np.loadtxt("{}/{}".format(path, file), dtype=np.float64, delimiter='\t') for file in wanted_files]

for i in range(0, len(data)):
    data[i][:,0] = data[i][:,0] - data[i][0,0]

fig, ax = plt.subplots(1, 1)
for i, x in enumerate(data):
    ax.plot(x[:,0], x[:,1], label=wanted_hs[i])

setAxisParameters(ax)

plt.show()
'''

PI = np.pi
A = 500
f = 0.5
t = np.linspace(0, 4 * PI, num=1000)
y1 = A * np.sin(2 * PI * f * t + PI/2) + A
y2 = A * 2 * PI * f * np.cos(2 * PI * f * t + PI/2)

plt.plot(t, y1)
plt.plot(t, y2)
plt.xlim(0, 4*PI)
plt.hlines(0, 0, 4*PI, color='k')
plt.show()
