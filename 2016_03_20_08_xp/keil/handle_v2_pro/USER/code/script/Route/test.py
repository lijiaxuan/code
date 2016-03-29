#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-25 17:45:31
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-25 17:45:57



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