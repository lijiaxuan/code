#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-02 09:15:23
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-17 16:57:01
import numpy as np
import matplotlib.pyplot as plt
import pdb

target = [{'x': 0, 'y': 3000}, 15]
log = open('path','w')
ANGLE = 0
DISTANCE = 1
pid  = 	{
			'pid_list': 
			[
				{
					'coff': 1,
					'kp': 	0.3,
					'ki': 	0,
					'kd': 	0,
				},
				{
					'coff':50,
					'kp': 	0.3,
					'ki': 	0,
					'kd': 	0,
				},
			],
			'mend':  [0, 0],
			'error': [[0,0], [0,0]]
		}
class Car():

    def __init__(self):
        pass
        self.point = {'x': 0, 'y': 0}
        self.angle = 0
        self.radian = 0
        self.step = 2
        self.path = [[0], [0]]

    def moveon(self):
        delta_x = np.cos(np.pi / 2 - self.radian)
        delta_y = np.sin(np.pi / 2 - self.radian)
        self.point['x'] += delta_x
        self.point['y'] += delta_y
        self.path[0].append(self.point['x'])
        self.path[1].append(self.point['y'])


def getpoint2line(carpostion, targetpoint, targetangle):
    k = np.tan(np.pi / 2 - targetangle * np.pi / 180.0)
    a = carpostion['y'] - k * carpostion['x'] + \
        k * targetpoint['x'] - targetpoint['y']
    return a / (np.sqrt(1 + k * k))


def getpid(car):
	#pdb.set_trace()
	d_error = getpoint2line(car.point,target[0],target[1])

	angle_error =target[1] - car.angle
	#pdb.set_trace()
	distance_error = d_error / pid['pid_list'][DISTANCE]['coff']
	if distance_error >= 80 :
		distance_error = 80
	elif distance_error <= -80:
		distance_error = -80
	log.write('  distance_error: '+ str(d_error) + '  angle_error : ' + str(angle_error) + '\r\n')
	A = pid['pid_list'][ANGLE]['kp']+pid['pid_list'][ANGLE]['ki']+pid['pid_list'][ANGLE]['kd']
	B = -(pid['pid_list'][ANGLE]['kp']+ 2*pid['pid_list'][ANGLE]['kd'])
	C = pid['pid_list'][ANGLE]['kd']
	
	angle_delta = A*angle_error #+ B*pid['error'][ANGLE][0] + C*pid['error'][ANGLE][1];
	pid['mend'][ANGLE] += angle_delta
	
	pid['error'][ANGLE][1] = pid['error'][ANGLE][0]
	pid['error'][ANGLE][0] = angle_error


	distance_error += angle_delta;
	A = pid['pid_list'][DISTANCE]['kp']+pid['pid_list'][DISTANCE]['ki']+pid['pid_list'][DISTANCE]['kd']
	B = -(pid['pid_list'][DISTANCE]['kp']+ 2*pid['pid_list'][DISTANCE]['kd'])
	C = pid['pid_list'][DISTANCE]['kd']
	
	distance_delta = A*distance_error #+ B*pid['error'][DISTANCE][0] + C*pid['error'][DISTANCE][1];
	
	pid['error'][DISTANCE][1] = pid['error'][DISTANCE][0]
	pid['error'][DISTANCE][0] = distance_error
	pid['mend'][DISTANCE] += distance_delta
	if pid['mend'][DISTANCE] >= 50:
		pid['mend'][DISTANCE] = 50
	elif pid['mend'][DISTANCE] <-50:
		pid['mend'][DISTANCE] = 50
	log.write('  distance_delta: '+ str(distance_delta) + '  angle_delta : ' + str(angle_delta) + '\r\n')
	log.write('  piderror: '+ str(pid['error']) + '\r\n')
	return pid['mend'][DISTANCE]

car = Car()
i = 1
while car.point['y'] < target[0]['y']:
	delta_angle = getpid(car)
	car.angle += delta_angle * 0.05
	#pdb.set_trace()
	car.angle = (car.angle + 180) % 360 - 180
	car.radian = car.angle * np.pi / 180
	car.moveon()
	log.write(str(car.point) + '  angle: ' + str(car.angle) + '  delta_angle: ' + str(delta_angle) + '\r\n\r\n')
	i = i + 1
	if i >= 5000:
		break
log.close()
plt.plot(car.path[0], car.path[1])
plt.show()
