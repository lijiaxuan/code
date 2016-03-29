#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2015-12-20 14:16:00
# @Last Modified by:   obsidian
# @Last Modified time: 2016-02-28 11:46:56
import numpy as np
import matplotlib.pyplot as plt
import random
y1 = []
y2 = []
k = [[0 for i in range(60)] for s in range(80)]
x = [i for i in range(80)]
for i in range(80):
    y1.append(int(i))
    print i,y1
    y2.append(int(i) + 15)
    if int(i) < 60:
        k[i+ random.randint(-3,3)][int(i )][0] = 15
    if int(i) < 45:
        k[i+ random.randint(-3,3)][int(i) + 12][0] = 31
# plt.plot(x,y1,'.')
# plt.plot(x,y2,'b.')
f = open('arr.c','w')
f.write('#include "arr.h" \n')
f.write('unsigned short arr[80][60] = ' + str(k).replace('],','],\n').replace('[','{').replace(']','}') + ';\n')
f.close()
plt.show()
