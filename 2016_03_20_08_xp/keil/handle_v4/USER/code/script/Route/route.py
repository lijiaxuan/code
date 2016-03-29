#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-25 15:20:00
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-30 10:27:52
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
		[[-2385.1198,5272.6007],0,0],
		[[-2624.9571,6087.2938],1,-1],
		[[-2241.2391,6600.2564],0,0],
		[[-1729.4402,6600.2564],1,1],
		[[-1356.8765,6740.5863],1,-1],
		[[-984.3007,6880.9161],0,0],
		[[126.5264,6880.91],1,1],
		[[419.7560,7008.8565],0,0],
		[[1657.5246,8342.9408],1,1],
		[[1651.2126,9254.2490],0,0],
		[[1003.1025,9933.6680],1,-1],
		[[1049.4999,10791.8316],0,0],
		[[1512.8070,11182.8239],1,1],
		[[1811.6046,11830.4200],0,0],
		[[1811.6046,12830.4200],0,0],
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
		[-2241.2391,6200.2564],
		[],
		[-1729.4402,7164.9998],
		[-984.3007,6316.1727],
		[],
		[126.5264,7280.9161],
		[],
		[1170.0299,8795.2402],
		[],
		[1430.0299,10340.9288],
		[],
		[970.0299,11825.9885],
		[],
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
	l = np.sqrt((start[0] - stop[0]) ** 2 + (start[1] - stop[1]) ** 2)
	for i in range(int(abs(l / step))):
		x.append(i * x_step + start[0])
		y.append(i * y_step + start[1])
		tmp = np.pi / 2 - t_l
		while tmp > np.pi:
			tmp -= 2 * np.pi
		while tmp < - np.pi:
			tmp += 2 * np.pi
		theta.append(tmp)
def circle(start,stop,center,direc):
	global x,y,theta
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
	if center[0] - stop[0] == 0:
		if stop[1] - center[1] > 0:
			theta_stop = np.pi / 2
		elif stop[1] - center[1] < 0:
			theta_stop = - np.pi / 2
	else:
		theta_stop = np.arctan((center[1] - stop[1]) / (center[0] - stop[0]))
		if stop[0] - center[0] < 0:
			theta_stop = theta_stop + np.pi
	if theta_start < 0:
		theta_start += 2 * np.pi
	if theta_stop < 0:
		theta_stop += 2 * np.pi
	if direc == -1:
		theta_step = -theta_step
		delta = theta_stop - theta_start
		while delta < 0:
			delta += 2 * np.pi
		delta =2 * np.pi - delta
	if direc == 1:
		theta_step = theta_step
		delta = theta_stop - theta_start
		while delta < 0:
			delta += 2 * np.pi
	#print delta 
	for i in range(int(abs(delta / theta_step)) + 1):
		x.append(center[0] + r * np.cos(theta_start + i * theta_step))
		y.append(center[1] + r * np.sin(theta_start + i * theta_step))
		
		if direc == 1 :
			tmp = - theta_start - i * theta_step
		if direc == -1 :
			tmp = np.pi - theta_start - i * theta_step
		while tmp > np.pi:
			tmp -= 2 * np.pi
		while tmp < - np.pi:
			tmp += 2 * np.pi
		theta.append(tmp)
left = [];
right = [];
for i in range(len(p) - 1):
	if p[i][1] == 0:
		line(p[i][0],p[i+1][0])
	else:
		circle(p[i][0],p[i+1][0],C[i],p[i][2])
for i in range(len(x)):
	right.append([[round(-x[i],4),round(y[i],4)],round(-theta[i],4)])
for i in range(len(x)):
	left.append([[round(x[i],4),round(y[i],4)],round(theta[i],4)])

f = open('Route_right.c', 'w')
f.write('#include \"../inc/pointpath.h\" \n struct Path_point path[PATH_LEN] = \n')
f.write(str(right).replace('[','{').replace(']','}').replace('}, {','},\n {') + ';\n')
f.close()
f = open('Route_left.c', 'w')
f.write('#include \"../inc/pointpath.h\" \n struct Path_point path[PATH_LEN] = \n')
f.write(str(left).replace('[','{').replace(']','}').replace('}, {','},\n {') + ';\n')
f.close()

plt.plot(x,y,'.')
#print y
plt.show()
