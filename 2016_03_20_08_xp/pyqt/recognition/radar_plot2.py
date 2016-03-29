import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1,polar = True)
line, = ax1.plot([], [],'b')
def init():
    line.set_data([], [])
    return line




x = []
y = []

def animate(i):
    global x,y
    global line
    x = [0]
    y = [0]
    f = open('point','r')
    c = f.read(-1)
    k = c.replace('[','').replace(']','').replace('(','').replace(' ','').split(')')
    for i in k:
        s = i.split(',')
        try:
            x.append(float(s[0]))
        except Exception:
            x.append(0)
        try:
            y.append(int(s[1]))
        except Exception:
            y.append(0)
        try:
            x.append(float(s[2]))
        except Exception:
            x.append(0)
        try:
            y.append(int(s[3]))
        except Exception:
            y.append(0)
        x.append(0)
        y.append(0)
    f.close()
    line.set_data(x,y)
    return line

anim1 = animation.FuncAnimation(
    fig, animate, init_func=init,  frames=50, interval=500)
plt.xlim(-8000,8000)
plt.ylim(0,16000)
plt.show()
