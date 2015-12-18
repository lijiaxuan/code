__author__ = 'sapphire'
import select
import socket
import os
server_num=9
sock=[]
FLAG=select.EPOLLET|select.EPOLLIN
epoller=select.epoll()
fd_socket={}
from data_ctl import update_matrix,caculater,data_ctler
updater=update_matrix()
caculate=caculater()
data_ctl=data_ctler()
import numpy as np
import os
iptype = np.dtype([('ip','S15'),('port','<i2')])

base_path=os.path.dirname(os.path.abspath(__file__))
bus_ip = np.fromfile(base_path+'/sensor/ip',dtype=iptype)
print bus_ip
def sock_init():
    for i in range(server_num):
        tmp=socket.socket()
        tmp.connect((str(bus_ip[i][0]),int(bus_ip[i][1])))
        epoller.register(tmp,FLAG)
        sock.append(tmp)
        fd_socket[tmp.fileno()]=tmp
import threading
import time
recvnum=0
data=[]
sock_init()
class worker1(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        global recvnum
        while True:
            try:
                recvnum=0
                for i in sock:
                    i.send("ASK")
                    print "ASK"
                time.sleep(4)
            except Exception:
                print Exception
                os._exit(1)
class worker2(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        global recvnum
        global sock
        global data
        while True:
            try:
                time.sleep(0.1)
                if recvnum==server_num:
                    data=caculate.cal()
                    for i in range(server_num):
                        _send=data_ctl.find_data(i+1,data)
                        sock[i].send(_send)
                    recvnum=0
            except Exception:
                print Exception
                os._exit(2)
s=worker1()
s.start()
s2=worker2()
s2.start()
def main():
    global recvnum
    global sock
    while True:
        events=epoller.poll()
        for fd,flag in events:
            s=fd_socket[fd]
            if flag&(select.EPOLLIN):
                try:
                    data=s.recv(512)
                    updater.update_matrix(data)
                    recvnum=recvnum+1
                except Exception:
                    os._exit(3)
main()
