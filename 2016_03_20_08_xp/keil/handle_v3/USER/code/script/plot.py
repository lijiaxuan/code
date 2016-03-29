#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-14 15:26:41
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-22 08:44:43
from matplotlib import pyplot as plt
from matplotlib import animation
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
# a = np.array([1,1])
# help(np.array.tofile('zero'))
#first set up the figure, the axis, and the plot element we want to animate
fig = plt.figure()
ax1 = fig.add_subplot(2, 1, 1, xlim=(0, 2), ylim=(-4, 4),projection='3d')
ax2 = fig.add_subplot(2, 1, 2, xlim=(0, 2), ylim=(-4, 4),projection='3d')
line, = ax1.plot([], [], lw=2)
line2, = ax2.plot([], [], lw=2)

#help(line.set_data)

def init():
    line.set_data([], [],[])
    line2.set_data([], [],[])
    return line, line2

# animation function.  this is called sequentially


def animate(i):

    x = np.linspace(0, 2, 100)
    y = np.sin(2 * np.pi * (x - 0.01 * i))
    line.set_data(x, y)

    x2 = np.linspace(0, 2, 100)
    y2 = np.cos(2 * np.pi * (x2 - 0.01 * i)) * \
        np.sin(2 * np.pi * (x - 0.01 * i))
    line2.set_data(x2, y2,[0])
    return line, line2

anim1 = animation.FuncAnimation(
    fig, animate, init_func=init,  frames=50, interval=10)
#ax1.plot([1,2],[2,3],[3,4])
plt.show()