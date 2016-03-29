import numpy as np
n = 256
f = open('mif','w')
for i in range(n):
    f.write(str(i) + ':' + str((int(128 * np.sin(2 * np.pi / n * i)) + 128 ))+ ';')
    f.write('\n')
