#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-25 15:20:00
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-25 18:50:19
import numpy as np
import matplotlib.pyplot as plt
x = []
y = []
theta = []
step = 5
p = \
	[
		[[0,0],0,0],
		[[0,800.0],1,1],
		[[-61.9861,1270.8309],0,0],
		[[-247.8,1964.36],1,1],
		[[-429,2403],0,0],
		[[-780.46,3010.9],1,1],
		[[-1068.6490,3386.0864],0,0],
		[[-1576.51,3893.789],1,-1],
		[[-1865.4486,4270.5],0,0],
		[[-2215.44,4876.76],1,-1],
		[[-2346.3640,5155.7123],0,0],
		[[-2645.054,5963.90],1,-1],
		[[-2086.68,6629.46],1,1],
		[[-1275.48,6800.06],1,-1],
		[[-984.3007,6880.9161],0,0],
		[[-1280.88,6880.91],1,1],
		[[544.5434,7160.0461],0,0],
		[[1632.4341,8250.7935],1,1],
		[[1641.7108,9159.3338],0,0],
		[[1019.96,9808.71],1,-1],
		[[1088.1284,10769.55],0,0],
		[[1503.96,11095.94],1,1],
		[[1810.0299,11725.2467],0,0],
	]
C = \
	[
		[],
		[-1819.1508,800.0000],
		[],
		[-2004.9829,1493.5347],
		[],
		[-2354.9829,2099.7525],
		[],
		[-290.0178,5180.1230],
		[],
		[-640.0178,5786.3407],
		[],
		[-2176.0588,6137.2345],
		[-1870.4908,7605.7808],
		[-984.3007,6316.1727],
		[],
		[-128.0883,7830.9161],
		[],
		[1172.2127,8709.8101],
		[],
		[1489.4533,10258.2406],
		[],
		[1010.0299,11725.2467],
		[]
	]

def line(start,stop):
	global x,y
	if stop[0] - start[0] == 0:
		if stop[1] - start[1] > 0:
			t_l = np.pi/2
		elif stop[1] - start[1] < 0:
			t_l = - np.pi / 2
	else:
		t_l = np.arctan((stop[1] - start[1]) / (stop[0] - start[0]))
	if stop[0] - start[0] < 0:
		t_l = t_l + np.pi	
	x_step = step * np.cos(t_l)
	y_step = step * np.sin(t_l)
	for i in range(int(abs((stop[1] - start[1]) / y_step))):
		x.append(i * x_step + start[0])
		y.append(i * y_step + start[1])
def circle(start,stop,center,direc):
	global x,y
	r = np.sqrt((center[1] - start[1]) * (center[1] - start[1]) + 
				(center[0] - start[0]) * (center[0] - start[0]))
	theta_step = step / r

	if center[0] - start[0] == 0:
		if start[1] - center[1] > 0:
			theta_start = np.pi/2
		elif start[1] - center[1] < 0:
			theta_start = - np.pi / 2
	else:
		theta_start = np.arctan((center[1] - start[1]) / (center[0] - start[0]))
		if start[0] - center[0] < 0:
			theta_start = theta_start + np.pi
		while theta_start > np.pi:
			theta_start -= 2 * np.pi
		while theta_start < -np.pi:
			theta_start += 2 * np.pi
	if center[0] - stop[0] == 0:
		if stop[1] - center[1] > 0:
			theta_stop = np.pi / 2
		elif stop[1] - center[1] < 0:
			theta_stop = - np.pi / 2
	else:
		theta_stop = np.arctan((center[1] - stop[1]) / (center[0] - stop[0]))
		if stop[0] - center[0] < 0:
			theta_stop = theta_stop + np.pi
		while theta_stop > np.pi:
			theta_stop -= 2 * np.pi
		while theta_stop < -np.pi:
			theta_stop += 2 * np.pi
	theta_start += 2 * np.pi
	theta_stop += 2 * np.pi
	if direc == -1:
		theta_step = -theta_step
	print center,theta_start,theta_stop
	for i in range(int(abs((theta_start - theta_stop) / theta_step)) + 1):
		x.append(center[0] + r * np.cos(theta_start + i * theta_step))
		y.append(center[1] + r * np.sin(theta_start + i * theta_step))
for i in range(len(p) - 1):
	if p[i][1] == 0:
		line(p[i][0],p[i+1][0])
	else:
		circle(p[i][0],p[i+1][0],C[i],p[i][2])
plt.plot(x,y,'.')
#print y
plt.show()
