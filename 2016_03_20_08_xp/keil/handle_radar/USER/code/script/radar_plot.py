import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
import traceback
fig = plt.figure()
ax1 = fig.add_subplot(1, 1, 1)
line, = ax1.plot([], [],'r*')
p1, = ax1.plot([], [],'r*')
cr1, = ax1.plot([],[],'g')
cr2, = ax1.plot([],[],'g')
cen, = ax1.plot([],[],'y.')

ls1, = ax1.plot([],[],'y-')
ls2, = ax1.plot([],[],'y-')
ls3, = ax1.plot([],[],'y-')
ls4, = ax1.plot([],[],'#5a3717',linewidth = 2)

path = \
    [
        [1526,1041,687,333,-20],
        [6584,7057,6705,7056,6705]
    ]
plt.plot(path[0],path[1],'#5c5c5c',linewidth=6)
t = []
def init():
    line.set_data([], [])

island = \
        [
            {'ord' : [787.88,7228.40 ], 'phi': 0, 'en' : 0},
            {'ord' : [687.88,7128.40 ], 'phi': 0, 'en' : 0} ,
            {'ord' : [587.88,7228.40 ], 'phi': 0, 'en' : 0},
            {'ord' : [81.21,7228.40  ], 'phi':  0, 'en'  : 0},
            {'ord' : [-19.21,7128.40 ], 'phi': 0, 'en'  : 0},
            {'ord' : [-119.34,7228.40], 'phi':0,'en' : 0},
            {'ord' : [234.34,6533.43 ], 'phi': 0,'en' : 0},
            {'ord' : [334.34,6633.43 ], 'phi': 0,'en' : 0},
            {'ord' : [434.34,6533.43 ], 'phi': 0,'en' : 0},
            {'ord' : [941.44,6533.43 ], 'phi':0,'en' : 0},
            {'ord' : [1041.44,6633.43], 'phi':0,'en' : 0},
            {'ord' : [1141.44,6533.43], 'phi':0,'en' : 0},
        ]


x = []
y = []

def rotate(arr,theta):
    a = [arr[0] * np.cos(theta) + arr[1] * np.sin(theta),
            -arr[0] * np.sin(theta) + arr[1] * np.cos(theta)]
    return a

def getord_mod2(isid):  # 0,2,5
    global island,x,y,cr1,cr2,cen
    c1 = [0,0]
    c2 = [0,0]
    res_x = []
    res_y = []
    t = []
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
    #print island[isid[0]]['phi'] , island[isid[1]]['phi']
    #print island[isid[2]]['phi'] , island[isid[3]]['phi']
    arr1 = rotate([-l1 ,l1 / np.tan(delta_theta1)], np.pi - t_l1)
    arr2 = rotate([-l2 ,l2 / np.tan(delta_theta2)], np.pi - t_l2)
    r1 = l1 / np.sin(delta_theta1)
    r2 = l2 / np.sin(delta_theta2)
    c1[0] = island[isid[1]]['ord'][0] + arr1[0]
    c1[1] = island[isid[1]]['ord'][1] + arr1[1]
    c2[0] = island[isid[3]]['ord'][0] + arr2[0]
    c2[1] = island[isid[3]]['ord'][1] + arr2[1]
    plt.plot([c1[0],c2[0]],[c1[1],c2[1]],'.')
    theta_l = np.arctan((c2[1] - c1[1]) / (c2[0] - c1[0]))
    if c2[0] - c1[0] < 0:
        theta_l = theta_l + np.pi
    cl = np.sqrt((c2[0] - c1[0]) * (c2[0] - c1[0]) + (c2[1] - c1[1]) * (c2[1] - c1[1]))
    cos_c = (r1 * r1 + cl * cl - r2 * r2) / (2 * r1 * cl)
    sin_c = np.sqrt(1 - cos_c * cos_c)
    #print cos_c,r1,r2,cl,theta_l
    [dx1,dy1] = rotate([ r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    #print dx1,dy1
    #print c1,c2
    cr1.set_data([c1[0] + r1 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c1[1] + r1 * np.sin(i * 2 * np.pi/100) for i in range(100)])

    cr2.set_data([c2[0] + r2 * np.cos(i * 2 * np.pi/100) for i in range(100)],[c2[1] + r2 * np.sin(i * 2 * np.pi/100) for i in range(100)])
    cen.set_data([c1[0],c2[0]],[c1[1],c2[1]])
    #if c1[0] + dx1 < 1048 and c1[1] + dy1 > 6533 and c1[1] + dy1 < 7288:
        #plt.plot([c1[0] + dx1],[c1[1] + dy1],'*')
        #plt.text(c1[0] + dx1,c1[1] + dy1+3,str(isid))
    res_x.append(c1[0] + dx1)
    res_y.append(c1[1] + dy1)
    t.append(np.arctan((c1[1] + dy1 - island[0]['ord'][1]) / (c1[0] + dx1 - island[0]['ord'][0])))
    if c1[0] + dx1 - island[0]['ord'][0] > 0:
        t[0] = t[0] + np.pi
    selft = t[0] - island[0]['phi']
    ls1.set_data([c1[0] + dx1,c1[0] + dx1 + 1000 * np.cos(t[0])],[c1[1] + dy1,c1[1] + dy1 + 1000 * np.sin(t[0])])
    t.append(t[0] + island[isid[1]]['phi'] - island[0]['phi'])
    ls2.set_data([c1[0] + dx1,c1[0] + dx1 + 1000 * np.cos(t[1])],[c1[1] + dy1,c1[1] + dy1 + 1000 * np.sin(t[1])])
    t.append(t[0] + island[isid[3]]['phi'] - island[0]['phi'])
    ls3.set_data([c1[0] + dx1,c1[0] + dx1 + 1000 * np.cos(t[2])],[c1[1] + dy1,c1[1] + dy1 + 1000 * np.sin(t[2])])
    #ls4.set_data([c1[0] + dx1,c1[0] + dx1 + 1000 * np.cos(selft)],[c1[1] + dy1,c1[1] + dy1 + 1000 * np.sin(selft)])
    print selft * 180 / np.pi
    print res_x,res_y
    # [dx2,dy2] = rotate([- r1 * sin_c , r1 * cos_c] ,-theta_l + np.pi / 2)
    # if c1[0] + dx2 < 1048 and c1[1] + dy2 > 6533 and c1[1] + dy2 < 7288:
    #     res_x.append(c1[0] + dx2)
    #     res_y.append(c1[1] + dy2)
    #     #plt.plot([c1[0] + dx2],[c1[1] + dy2],'*')
        #plt.text(c1[0] + dx2,c1[1] + dy2+3,str(isid))
    return [res_x,res_y]
def animate(i):
    global x,y,ls4
    global line
    x = [0]
    y = [0]

    # for i in range(6):
    #     island[i]['en'] = 0
    try:
        f = open('point','r')
        c = f.read(-1)
        k = eval(c.replace(')','),'))
        for i in range(len(k)):
            if k[i][2] == 1.0:
                island[0]['phi'] = -0.762972160237 #k[i][0]
                island[2]['phi'] = -0.0495 #k[i][1]
            if k[i][2] == 2.0:
                island[3]['phi'] = -0.256497 + 0.01 #k[i][0]
            if k[i][2] == 3.0:
                island[7]['phi'] = 0.440337646171 #k[i][0]
                island[8]['phi'] = k[i][1]

                #island[3]['en'] = 1
            # if k[i][2] == 4:
            #     island[4]['phi'] = k[i][1]
            #     island[5]['phi'] = k[i][0]
            #     island[4]['en'] = 1
            #     island[5]['en'] = 1
        f.close()
        # arr = getord_mod2([0,1,4,5])
        # x = x + arr[0]
        # y = y + arr[1]
        # arr = getord_mod2([1,2,3,4])
        # x = x + arr[0]
        # y = y + arr[1]
        # arr = getord_mod2([1,2,1,5])
        # x = x + arr[0]
        # y = y + arr[1]
        arr = getord_mod2([0,3,0,7])
        x = x + arr[0]
        y = y + arr[1]
        #line.set_data(x,y)
        # x.append(k[3][0])
        # y.append(k[3][1])
        # r = k[3][2]
        # line.set_data(x,y)
        # ls4.set_data([x[2], x[2] + 1000 * np.cos(r)],[y[2],y[2] + 1000 * np.sin(r)])
    except Exception:
        pass
        #print traceback.print_exc()
    return line


plt.plot([island[6]['ord'][0],island[7]['ord'][0],island[8]['ord'][0]],
        [island[6]['ord'][1],island[7]['ord'][1],island[8]['ord'][1]],
        'b'
        )

plt.plot([island[9]['ord'][0],island[10]['ord'][0],island[11]['ord'][0]],
        [island[9]['ord'][1],island[10]['ord'][1],island[11]['ord'][1]],
        'b'
        )
plt.plot([island[0]['ord'][0],island[1]['ord'][0],island[2]['ord'][0]],
        [island[0]['ord'][1],island[1]['ord'][1],island[2]['ord'][1]],
        'b'
        )

plt.plot([island[3]['ord'][0],island[4]['ord'][0],island[5]['ord'][0]],
        [island[3]['ord'][1],island[4]['ord'][1],island[5]['ord'][1]],
        'b'
        )
animate(0)
# anim1 = animation.FuncAnimation(
#      fig, animate, init_func=init,  frames=50, interval=500)
plt.ylim(6000,8000)
plt.xlim(-200,1800)
plt.show()
