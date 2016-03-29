#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-21 15:36:50
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-21 16:18:33
import numpy as np
n = 4096
point = []
step = 2 * np.pi / n
for i in range(n):
	k = int(np.sin(i * step) * 2048)  + 2048
	if k >= 4096:
		k = 4095
	point.append(k)
print "unsigned short sin_[4096] = ",str(point).replace(']','}').replace('[','{'),';'
