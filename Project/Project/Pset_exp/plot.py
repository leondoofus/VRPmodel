import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as lines
import matplotlib.transforms as mtransforms
import matplotlib.text as mtext

table = [[],[],[],[],[]]

f = open("result.txt", 'r')
for l in f:
    line = l.split()
    for i in range(len(line)):
        if i == 0:
            table[i].append(line[i][0:-4])
        else:
            table[i].append(float(line[i]))
f.close()

fig, ax = plt.subplots()
bar_width = 0.2
opacity = 0.4

index = np.arange(len(table[0]))

rects1 = ax.bar(index, table[1], bar_width,
                alpha=opacity, color='g',
                label='OPT')

rects2 = ax.bar(index + bar_width, table[2], bar_width,
                alpha=opacity, color='r',
                label='CONST')

rects3 = ax.bar(index + 2 * bar_width, table[3], bar_width,
                alpha=opacity, color='b',
                label='SA')

rects4 = ax.bar(index + 3 * bar_width, table[4], bar_width,
                alpha=opacity, color='y',
                label='PLNE')

ax.set_ylabel('Value')
ax.set_title('P set')
ax.set_xticks(index + bar_width / 2)
ax.set_xticklabels(table[0])
plt.xticks(rotation=90)
ax.legend()

fig.tight_layout()
plt.show()