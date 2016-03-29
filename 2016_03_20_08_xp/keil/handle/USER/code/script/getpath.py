#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2015-12-16 17:53:12
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-03 17:39:28
# import numpy as np
# x = []
# array_len = 200
# step = np.pi/array_len
# R = 500
# print step
# for i in range(array_len):
# 	x.append([[round(R*np.sin(i * step),6),round(500 - R*np.cos(i*step),6)], round((i*step*180/np.pi + 90)%360,6)])
# str_x = str(x)
# print 'struct Path_point path[PATH_LEN] = \n', str_x.replace('[','{').replace('],','},\n').replace(']]','\n}};')
k = open('/home/obsidian/virtualbox/xp/code/keil/handle/USER/code/control/GPSpath/src/pointpath.c','r')
content = k.read(-1)
s = content.split('{{')
print s