#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-06 19:59:12
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-06 20:12:53
import numpy as np
a = np.array([[1,0,3],[3,4,5]])
nonzero = np.nonzero(a)
indice = open('zero','w')

for i in range(len(nonzero[0])):
	indice.write(str([nonzero[0][i],nonzero[1][i]]).replace('[','{').replace(']','}') + ', ')
indice.close()