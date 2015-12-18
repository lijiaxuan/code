__author__ = 'sapphire'

import select
import socket
import threading
import time
import random
from twisted.internet import reactor,error
server_num=9
sock=[]
FLAG=select.EPOLLET|select.EPOLLIN
epoller=select.epoll()
fd_socket={}
import numpy as np
from scipy import interpolate
from pd_sample import pd_sample
import datetime

ynew=[]
qnew=[]
pdtype =np.dtype([('bus_id','<i4'),('pd','<f4'),('qd','<f4')],align=True)
last_hour=0
hourpd=[]
hourqd=[]
last_refresh_time=0
pd=0
qd=0
import os
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
iptype = np.dtype([('ip','S15'),('port','<i2')])
bus_ip = np.fromfile(BASE_DIR+'/ip',dtype=iptype)
def sock_init():
    for i in range(server_num):
        try:
            tmp=socket.socket()
            tmp.connect((str(bus_ip[i][0]),int(bus_ip[i][1])+1))
            print (str(bus_ip[i][0]),int(bus_ip[i][1])+1)
            sock.append(tmp)
        except Exception:
            os._exit(1)
recvnum=0
sock_init()
power=[[97.6, 44.2], [30, 30], [322, 2.4], [500, 184], [30, 30], [30, 30], [233.8, 84], [522, 176.6], [6.5, -66.6], [30, 30], [30, 30], [8.53, 88], [30, 30], [30, 30], [320, 153], [329, 32.3], [30, 30], [158, 30], [30, 30], [680, 103], [274, 115], [30, 30], [247.5, 84.6], [308.6, -92.2], [224, 47.2], [139, 17], [281, 75.5], [206, 27.6], [283.5, 26.9], [30, 30], [9.2, 4.6], [30, 30], [30, 30], [30, 30], [30, 30], [30, 30], [30, 30], [30, 30], [1104, 250]]

class worker1(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        global recvnum
        while True:
            try:
                recvnum=0
                for i,k in enumerate(sock):
                    a=np.array([(i,power[i][0]+random.random()*5.0,power[i][1]+3.0*random.random())],dtype=pdtype)
                    k.send(a.tobytes())
                time.sleep(3.5)
            except Exception:
                os._exit(2)

s=worker1()
s.start()
reactor.run()