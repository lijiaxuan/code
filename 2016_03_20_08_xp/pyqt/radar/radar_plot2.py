import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)
line, = ax1.plot([], [],'r*')
p1, = ax1.plot([], [],'r*')
def init():
    line.set_data([], [])

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

def rotate(arr,theta):
    a = [arr[0] * np.cos(theta) + arr[1] * np.sin(theta),
            -arr[0] * np.sin(theta) + arr[1] * np.cos(theta)]
    return a

def getord_mod2(isid):  # 0,2,5
    global island,x,y
    c1 = [0,0]
    c2 = [0,0]
    res_x = []
    res_y = []
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
    #print cos_c,r1,r2,cl
    [dx1,dy1] = rotate([ r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    #plt.plot([c1[0] + r1 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c1[1] + r1 * np.sin(i * 2 * np.pi/100) for i in range(100)])
    #plt.plot([c2[0] + r2 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c2[1] + r2 * np.sin(i * 2 * np.pi/100) for i in range(100)])

    if c1[0] + dx1 > 1048 and c1[1] + dy1 > 6533 and c1[1] + dy1 < 7288:
        #plt.plot([c1[0] + dx1],[c1[1] + dy1],'*')
        #plt.text(c1[0] + dx1,c1[1] + dy1+3,str(isid))
        res_x.append(c1[0] + dx1)
        res_y.append(c1[1] + dy1)
    [dx2,dy2] = rotate([- r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    if c1[0] + dx2 > 1048 and c1[1] + dy2 > 6533 and c1[1] + dy2 < 7288:
        res_x.append(c1[0] + dx2)
        res_y.append(c1[1] + dy2)
        #plt.plot([c1[0] + dx2],[c1[1] + dy2],'*')
        #plt.text(c1[0] + dx2,c1[1] + dy2+3,str(isid))
    return [res_x,res_y]
def animate(i):
    global x,y
    global line
    x = [0]
    y = [0]
    try:
        f = open('point','r')
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
        # arr = getord_mod2([0,1,4,5])
        # x = x + arr[0]
        # y = y + arr[1]
        arr = getord_mod2([1,2,3,4])
        x = x + arr[0]
        y = y + arr[1]
        arr = getord_mod2([1,2,1,5])
        x = x + arr[0]
        y = y + arr[1]
        arr = getord_mod2([0,2,0,5])
        x = x + arr[0]
        y = y + arr[1]
        line.set_data(x,y)
    except Exception:
        pass
    return line


plt.plot([island[0]['ord'][0],island[1]['ord'][0],island[2]['ord'][0]],
		[island[0]['ord'][1],island[1]['ord'][1],island[2]['ord'][1]],
		'b'
		)

plt.plot([island[3]['ord'][0],island[4]['ord'][0],island[5]['ord'][0]],
        [island[3]['ord'][1],island[4]['ord'][1],island[5]['ord'][1]],
        'b'
        )



anim1 = animation.FuncAnimation(
    fig, animate, init_func=init,  frames=50, interval=500)
plt.ylim(6000,7800)
plt.xlim(0,1800)
plt.show()
