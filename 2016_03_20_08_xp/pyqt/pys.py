import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)
line, = ax1.plot([], [],'b.')
nop, = ax1.plot([], [],'y*')
target, = ax1.plot([], [],'g.')
def init():
    line.set_data([], [])
    return line




x = []
y = []

path_x = []
path_y = []


f = open('E:\code\keil\handle_v2\USER\code\control\GPSpath\src\pointpath.c','r')
k = f.read(-1).split('=')[1]
item =  k.split('},\n')
for i in item:
    try:
        tmp_l = i.split('{ {')[1].split('},')[0].split(',')
        #print tmp_l
        try:
           path_x.append(float(tmp_l[0]))
        except Exception:
            path_x.append(0)
        try:
            path_y.append(float(tmp_l[1]))
        except Exception:
            path_y.append(0)
    except Exception:
        pass

#plt.plot(x,y,'b')
ax1.plot(path_x,path_y,'r')

path_x1 = []
path_y1 = []

target_x = 0
target_y = 0
k = 0
def animate(i):
    global x,y
    global target_x,target_y
    global line
    x = []
    y = []
    try:
        f = open('point','r')
        c = f.read(-1)
        k = c.replace(')','').replace('(','').split('\n')[1:]
        for i in k:
            s = i.split(',')
            try:
                x.append(float(s[0]))
            except Exception:
                pass
            try:
                y.append(float(s[1]))
            except Exception:
                pass
            try:
                target_x = float(s[5])
            except Exception:
                pass
            try:
                target_y = float(s[6])
            except Exception:
                pass
        f.close()
        #print target_x,target_y
        nop.set_data([x[len(x) - 1]],[y[len(y) - 1]])
        target.set_data([target_x],[target_y])
    except Exception:
        pass
    line.set_data(x,y)
    return line

anim1 = animation.FuncAnimation(
    fig, animate, init_func=init,  frames=50, interval=80)
plt.xlim(-8000,8000)
plt.ylim(0,16000)
plt.show()
