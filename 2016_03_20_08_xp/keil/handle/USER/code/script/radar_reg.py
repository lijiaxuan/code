#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-24 10:06:14
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-24 10:11:50
#65
import matplotlib.pyplot as plt
import numpy as np

x = [0]
y = [0]
theta_x = []
p_y = []

f = open('point','r')
c = f.read(-1)
k = c.replace('[','').replace(']','').replace('(','').replace(' ','').replace('\r\n','').split(')')
for i in k:
    s = i.split(',')
    try:
        x.append(float(float(s[1]) * np.cos(float(s[0]))))
        theta_x.append(float(s[0]))
    except Exception:
        x.append(0)
    try:
        y.append(float(float(s[1]) * np.sin(float(s[0]))))
        p_y.append(int(s[1]))
    except Exception:
        y.append(0)
    try:
        x.append(float(float(s[3]) * np.cos(float(s[2]))))
        theta_x.append(float(s[2]))
    except Exception:
        x.append(0)
    try:
        y.append(float(float(s[3]) * np.sin(float(s[2]))))
        p_y.append(int(s[3]))
    except Exception:
        y.append(0)
f.close()
print theta_x
print np.arctan((y[1] - y[0]) / (x[1] - x[0])) * 180 / np.pi
plt.plot(x,y)
plt.show()