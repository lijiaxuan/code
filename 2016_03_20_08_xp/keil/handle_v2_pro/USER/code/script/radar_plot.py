#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Author: obsidian
# @Date:   2016-01-23 09:48:47
# @Last Modified by:   obsidian
# @Last Modified time: 2016-01-26 20:43:04
# among
# 
import matplotlib.pyplot as plt
import numpy as np
island = \
        [
            {'ord' : [787.88,7228.40], 'phi':  -0.30066022276878357, 'en' : 0},
            {'ord' : [687.88,7128.40], 'phi':  -0.14112623035907745, 'en' : 0} ,
            {'ord' : [-19.21,7128.40], 'phi':  0.06749515235424042, 'en'  : 0},
            {'ord' : [334.34,6633.43], 'phi': 0.429514616727829,'en' : 0},
            {'ord' : [1041.44,6633.43], 'phi':0.5951845049858093,'en' : 0},
            {'ord' :[1141.44,6533.43], 'phi':0.9326602816581726,'en' : 0},
        ]
x = []
y = []

l = 19.21 + 687.88

ls = l / 2

theta_x = []
p_y = []

is_point = []
is_point.append([])
is_point.append([])

def rotate(arr,theta):
    a = [arr[0] * np.cos(theta) + arr[1] * np.sin(theta),
            -arr[0] * np.sin(theta) + arr[1] * np.cos(theta)]
    return a
def getord_mod1(isid):  # 0,1,4,5
    global island
    c1 = [0,0]
    c2 = [0,0]
    l = np.sqrt((island[0]['ord'][0] - island[1]['ord'][0]) * (island[0]['ord'][0] - island[1]['ord'][0]) + (island[0]['ord'][1] - island[1]['ord'][1])* (island[0]['ord'][1] - island[1]['ord'][1])) / 2
    #l2 = np.sqrt((island[0]))
    delta_theta1 = abs(island[0]['phi'] - island[1]['phi'])
    delta_theta2 = abs(island[4]['phi'] - island[5]['phi'])
    arr1 = rotate([l ,l / np.tan(delta_theta1)], np.pi * 3 / 4)
    arr2 = rotate([-l ,l / np.tan(delta_theta2)],np.pi/4)
    r1 = l / np.sin(delta_theta1)
    r2 = l / np.sin(delta_theta2)
    c1[0] = island[0]['ord'][0] + arr1[0]
    c1[1] = island[0]['ord'][1] + arr1[1]
    c2[0] = island[5]['ord'][0] + arr2[0]
    c2[1] = island[5]['ord'][1] + arr2[1]
    plt.plot([c1[0],c2[0]],[c1[1],c2[1]],'.')
    theta_l = np.arctan((c2[1] - c1[1]) / (c2[0] - c1[0]))
    if c2[0] - c1[0] < 0:
        theta_l = theta_l + np.pi 
    cl = np.sqrt((c2[0] - c1[0]) * (c2[0] - c1[0]) + (c2[1] - c1[1]) * (c2[1] - c1[1]))

    cos_c = (r1 * r1 + cl * cl - r2 * r2) / (2 * r1 * cl)
    sin_c = np.sqrt(1 - cos_c * cos_c)
    [dx1,dy1] = rotate([ r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    
    plt.plot([c1[0] + r1 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c1[1] + r1 * np.sin(i * 2 * np.pi/100) for i in range(100)])
    plt.plot([c2[0] + r2 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c2[1] + r2 * np.sin(i * 2 * np.pi/100) for i in range(100)])
    plt.plot([c1[0],c1[0] + dx1],[c1[1],c1[1] + dy1])
    [dx2,dy2] = rotate([- r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    plt.plot([c1[0],c1[0] + dx2],[c1[1],c1[1] + dy2])

def getord_mod2(isid):  # 0,2,5
    global island
    c1 = [0,0]
    c2 = [0,0]
    l1 = np.sqrt((island[isid[0]]['ord'][0] - island[isid[1]]['ord'][0]) **2 + (island[isid[0]]['ord'][1] - island[isid[1]]['ord'][1])**2 ) / 2
    l2 = np.sqrt((island[isid[2]]['ord'][0] - island[isid[3]]['ord'][0]) **2 + (island[isid[2]]['ord'][1] - island[isid[3]]['ord'][1])** 2) / 2
    t_l1 = np.arctan((island[isid[0]]['ord'][1] - island[isid[1]]['ord'][1]) / (island[isid[0]]['ord'][0] - island[isid[1]]['ord'][0]))
    if island[isid[0]]['ord'][0] - island[isid[1]]['ord'][0]  < 0:
        t_l1 += np.pi
    t_l2 = np.arctan((island[isid[2]]['ord'][1] - island[isid[3]]['ord'][1]) / (island[isid[2]]['ord'][0] - island[isid[3]]['ord'][0]))
    if island[isid[2]]['ord'][0] - island[isid[3]]['ord'][0]  < 0:
        t_l2 += np.pi

    delta_theta1 = abs(island[isid[0]]['phi'] - island[isid[1]]['phi'])
    delta_theta2 = abs(island[isid[2]]['phi'] - island[isid[3]]['phi'])
    arr1 = rotate([-l1 ,l1 / np.tan(delta_theta1)], np.pi - t_l1)
    arr2 = rotate([-l2 ,l2 / np.tan(delta_theta2)], np.pi - t_l2)
    r1 = l1 / np.sin(delta_theta1)
    r2 = l2 / np.sin(delta_theta2)
    c1[0] = island[isid[1]]['ord'][0] + arr1[0]
    c1[1] = island[isid[1]]['ord'][1] + arr1[1]
    c2[0] = island[isid[3]]['ord'][0] + arr2[0]
    c2[1] = island[isid[3]]['ord'][1] + arr2[1]
    #plt.plot([c1[0],c2[0]],[c1[1],c2[1]],'.')
    theta_l = np.arctan((c2[1] - c1[1]) / (c2[0] - c1[0]))
    if c2[0] - c1[0] < 0:
        theta_l = theta_l + np.pi 
    cl = np.sqrt((c2[0] - c1[0]) * (c2[0] - c1[0]) + (c2[1] - c1[1]) * (c2[1] - c1[1]))

    cos_c = (r1 * r1 + cl * cl - r2 * r2) / (2 * r1 * cl)
    sin_c = np.sqrt(1 - cos_c * cos_c)
    [dx1,dy1] = rotate([ r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    
    # plt.plot([c1[0] + r1 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c1[1] + r1 * np.sin(i * 2 * np.pi/100) for i in range(100)])
    # plt.plot([c2[0] + r2 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c2[1] + r2 * np.sin(i * 2 * np.pi/100) for i in range(100)])
    
    if c1[0] + dx1 > 1048 and c1[1] + dy1 > 6533 and c1[1] + dy1 < 7288:
        plt.plot([c1[0] + dx1],[c1[1] + dy1],'*')
        plt.text(c1[0] + dx1,c1[1] + dy1+3,str(isid))
    [dx2,dy2] = rotate([- r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    if c1[0] + dx2 > 1048 and c1[1] + dy2 > 6533 and c1[1] + dy2 < 7288:
        plt.plot([c1[0] + dx2],[c1[1] + dy2],'*')
        plt.text(c1[0] + dx2,c1[1] + dy2+3,str(isid))
f = open('point_3','r')
c = f.read(-1)
k = eval(c.replace(')','),'))
for i in range(6):
    island[i]['en'] = 0
for i in range(4):
    if k[0][i][2] == 1:
        island[0]['phi'] = k[0][i][1]
        island[1]['phi'] = k[0][i][0]
        island[0]['en'] = 1
        island[1]['en'] = 1
    if k[0][i][2] == 2:
        island[2]['phi'] = k[0][i][0]
        island[2]['en'] = 1
    if k[0][i][2] == 3:
        island[3]['phi'] = k[0][i][1]
        island[3]['en'] = 1
    if k[0][i][2] == 4:
        island[4]['phi'] = k[0][i][1]
        island[5]['phi'] = k[0][i][0]
        island[4]['en'] = 1
        island[5]['en'] = 1

f.close()
# print island
#c.replace('[','').replace(']','').replace('(','').replace(' ','').replace('\r\n','').split(')')
# for i in range(len(k)):
#     s = k[i].split(',')
#     try:
#         island[6 - i]['phi'] = float(s[0])
#     except Exception:

#     try:
        
#     except Exception:
f.close()
plt.plot(x,y,'r*')
plt.plot([island[0]['ord'][0],island[1]['ord'][0],island[2]['ord'][0]],
		[island[0]['ord'][1],island[1]['ord'][1],island[2]['ord'][1]],
		'b'
		)

plt.plot([island[3]['ord'][0],island[4]['ord'][0],island[5]['ord'][0]],
        [island[3]['ord'][1],island[4]['ord'][1],island[5]['ord'][1]],
        'b'
        )

# is_point[0].append(theta_x[1])
# is_point[0].append(theta_x[3])
# is_point[0].append(theta_x[4])
# is_point[0].append(theta_x[6])

# is_point[1].append(p_y[1])
# is_point[1].append(p_y[3])
# is_point[1].append(p_y[4])
# is_point[1].append(p_y[6])

# delta_theta1 = abs(island[1]['phi'] - island[2]['phi'])

# delta_theta2 = abs(island[3]['phi'] - island[4]['phi'])

# t1 = ls / np.tan(delta_theta1)
# t2 = ls / np.tan(delta_theta2)

# r1 = ls / np.sin(delta_theta1)
# r2 = ls / np.sin(delta_theta2)

# c1 = [0,0]
# c2 = [0,0]

# c1[0] = island[1]['ord'][0] - ls
# c1[1] = island[1]['ord'][1] - t1

# c2[0] = island[4]['ord'][0] - ls
# c2[1] = island[4]['ord'][1] + t2

# theta_l = np.arctan((c2[1] - c1[1]) / (c2[0] - c1[0])) + np.pi/2
# cl = np.sqrt((c2[0] - c1[0]) * (c2[0] - c1[0]) + (c2[1] - c1[1]) * (c2[1] - c1[1]))

# cos_c = (r2 * r2 + cl * cl - r1 * r1) / (2 * r2 * cl)
# sin_c = np.sqrt(1 - cos_c * cos_c)

# plt.plot([c1[0] + r1 * np.cos(2 * i * np.pi/1000) for i in range(1000)],[c1[1] + r1 * np.sin(2 * i * np.pi/1000) for i in range(1000)])
# plt.plot([c2[0] + r2 * np.cos(2 * i * np.pi/1000) for i in range(1000)],[c2[1] + r2 * np.sin(2 * i * np.pi/1000) for i in range(1000)])


# plt.plot([c1[0],c2[0]],[c1[1],c2[1]],'g.')

# y_ = - r2 * cos_c
# x_ = r2 * sin_c


# plt.plot([c2[0] + 0,c2[0] + x_],[c2[1],c2[1] + y_])

# xs = x_ * np.cos(-theta_l) + y_ * np.sin(-theta_l)
# ys = -x_ * np.sin(-theta_l) + y_ * np.cos(-theta_l)

# pit = [0,0]
# pit[0] = c2[0] - xs
# pit[1] = c2[1] - ys

# plt.plot(pit[0],pit[1],'y*')

# phi =np.pi/2 -  (np.arctan((pit[0] - island[0][0])/(island[0][1] - pit[1] )) - is_point[0][0] )
# print phi * 180 /np.pi
#getord_mod1(0)
getord_mod2([1,2,3,4])
getord_mod2([0,2,0,5])
#getord_mod2([0,1,4,5])
getord_mod2([1,2,1,5])
# arr = [100,100]
# plt.plot([0,arr[0]],[0,arr[1]])
# s = rotate(arr,np.pi * 3 / 4)
# plt.plot([0,s[0]],[0,s[1]],'r')
# plt.plot()
plt.show()
