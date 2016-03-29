#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-23 09:48:47
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-23 19:32:55
import matplotlib.pyplot as plt
import numpy as np
island = [[687.88,7128.40],
    [-19.21,7128.40],
    [1041.44,6633.4],
	[334.34,6633.43],]
x = []
y = []

l = 19.21 + 687.88

ls = l / 2

theta_x = []
p_y = []

is_point = []
is_point.append([])
is_point.append([])

f = open('point_2','r')
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
    # x.append(0)
    # y.append(0)
f.close()
# plt.plot(x,y,'r*')
plt.plot([island[0][0],island[1][0],island[2][0],island[3][0]],
		[island[0][1],island[1][1],island[2][1],island[3][1]],
		'r*'
		)
is_point[0].append(theta_x[1])
is_point[0].append(theta_x[3])
is_point[0].append(theta_x[4])
is_point[0].append(theta_x[6])

is_point[1].append(p_y[1])
is_point[1].append(p_y[3])
is_point[1].append(p_y[4])
is_point[1].append(p_y[6])

delta_theta1 = is_point[0][1] - is_point[0][0]

delta_theta2 = is_point[0][3] - is_point[0][2]

t1 = ls / np.tan(delta_theta1)
t2 = ls / np.tan(delta_theta2)

r1 = ls / np.sin(delta_theta1)
r2 = ls / np.sin(delta_theta2)

c1 = [0,0]
c2 = [0,0]

c1[0] = island[0][0] - ls
c1[1] = island[0][1] - t1

c2[0] = island[2][0] - ls
c2[1] = island[2][1] + t2

theta_l = np.arctan((c2[1] - c1[1]) / (c2[0] - c1[0])) + np.pi/2
cl = np.sqrt((c2[0] - c1[0]) * (c2[0] - c1[0]) + (c2[1] - c1[1]) * (c2[1] - c1[1]))

cos_c = (r2 * r2 + cl * cl - r1 * r1) / (2 * r2 * cl)
sin_c = np.sqrt(1 - cos_c * cos_c)


print np.arcsin(sin_c) * 180 / np.pi
print theta_l * 180 / np.pi

plt.plot([c1[0] + r1 * np.cos(2 * i * np.pi/1000) for i in range(1000)],[c1[1] + r1 * np.sin(2 * i * np.pi/1000) for i in range(1000)])
plt.plot([c2[0] + r2 * np.cos(2 * i * np.pi/1000) for i in range(1000)],[c2[1] + r2 * np.sin(2 * i * np.pi/1000) for i in range(1000)])


plt.plot([c1[0],c2[0]],[c1[1],c2[1]],'g.')

y_ = - r2 * cos_c
x_ = r2 * sin_c


plt.plot([c2[0] + 0,c2[0] + x_],[c2[1],c2[1] + y_])

xs = x_ * np.cos(-theta_l) + y_ * np.sin(-theta_l)
ys = -x_ * np.sin(-theta_l) + y_ * np.cos(-theta_l)

pit = [0,0]
pit[0] = c2[0] - xs
pit[1] = c2[1] - ys

plt.plot(pit[0],pit[1],'y*')
print "delta_theta1 = ",delta_theta1
print 'c1 = ',c1
print 'c2 = ',c2
print 't1 ',t1
print 't2 ',t2
print 'x_',x_
print 'y_',y_
print pit

phi =np.pi/2 -  (np.arctan((pit[0] - island[0][0])/(island[0][1] - pit[1] )) - is_point[0][0] )
print phi * 180 /np.pi
plt.show()
