import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
#ax1 = fig.gca(projection='3d')
ax1 = fig.add_subplot(221, projection='3d')
ax2 = fig.add_subplot(222, projection='3d')
ax3 = fig.add_subplot(223, projection='3d')


x = []
y = []
distance_error = []
angle_error = []
mend= []

path_x = []
path_y = []


f = open('E:\code\keil\handle\USER\code\control\GPSpath\src\pointpath.c','r')
k = f.read(-1).split('=')[1]
item =  k.split('},\n')
for i in item:
    try:
        tmp_l = i.split('{ {')[1].split('},')[0].split(',')
        #print tmp_l
        try:
           path_x.append(float(tmp_l[0]))
        except Exception:
            path_x.append(0)
        try:
            path_y.append(float(tmp_l[1]))
        except Exception:
            path_y.append(0)
    except Exception:
        pass

#plt.plot(x,y,'b')
ax1.plot(path_x,path_y,'r')

f = open('point','r')
c = f.read(-1)
k = c.replace(')','').replace('(','').split('\n')[1:]
for i in k:
    s = i.split(',')
    try:
        x.append(float(s[0]))
    except Exception:
        pass
    try:
        y.append(float(s[1]))
    except Exception:
        pass
    try:
        distance_error.append(float(s[2]))
    except Exception:
        pass
    try:
        angle_error.append(float(s[3]))
    except Exception:
        pass #z.append(0)
    try:
        mend.append(float(s[4]))
    except Exception:
        pass
f.close()
ax1.plot(x,y,distance_error,'y')
ax1.set_title('distance')
ax2.plot(path_x,path_y,'r')
ax2.plot(x,y,mend,'g')
ax2.set_title('mend')
ax3.plot(path_x,path_y,'r')
ax3.plot(x,y,angle_error,'b')
ax3.set_title('angle')
plt.xlim(-8000,8000)
plt.ylim(0,16000)
print distance_error
plt.show()
