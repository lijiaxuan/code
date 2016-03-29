#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-16 10:28:55
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-16 14:00:29
import numpy as np
import struct
import matplotlib.pyplot as plt
num = []
result = []
f = open('/home/obsidian/virtualbox/xp/radar/rawdata','r')
content = f.read(-1)
p = content.split('0RHSM')
data = p[1].split('down')[0]
k = data.replace(struct.pack('2B',0x0d,0x0a),'')
e = 0
for i in range(len(k)):
	if ( i+1 )%65 == 0:
		continue
	# if e == 0:
	# 	print '  '
	e = (e + 1)%3
	s = ord(k[i]) - 0x30
	# print s,ord(k[i]),k[i],' ',i
	num.append(s)
for i in range(len(num)/3):
	#print num[i*3],num[i*3+1],num[i*3+2]
	re = (num[i*3] << 12) + (num[i*3 + 1] << 6) + (num[i*3 + 2] << 0)
	result.append(re)
#print len(result)
step = np.pi/len(result)
x = [np.pi/3 + step*i for i in range(len(result))]
plt.subplot(111,polar = True)
plt.plot(x,result)
plt.show()