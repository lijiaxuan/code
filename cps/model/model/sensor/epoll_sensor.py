__author__ = 'sapphire'
import socket
import select
import socket
server_num=9
sock=[]
FLAG=select.EPOLLET|select.EPOLLIN
epoller=select.epoll()
fd_socket={}
import numpy as np
from connector import update_bus_data
import os
iptype = np.dtype([('ip','S15'),('port','<i2')])

base_path=os.path.dirname(os.path.abspath(__file__))
bus_ip = np.fromfile(base_path+'/ip',dtype=iptype)
ynew=[]
qnew=[]
persontype =np.dtype([('bus_id','<i2'),('type','i1'),('status','i1'),('pg','<f4')
    ,('qg','<f4'),('pd','<f4'),('qd','<f4'),('vm','<f4'),('va','<f4'),('branchnum','i1'),('al','i1'),('gen_id','<i2')
    ,('branchdata',[('is_from','i1'),('branch_status','i1'),('bus_from','<i2'),('bus_to','<i2'),('branch_id','<i2'),('p','<f4'),('q','<f4')],4)],align=True)
key=['bus_id', 'type', 'status', 'pg', 'qg', 'pd', 'qd', 'vm', 'va', 'branchnum', 'al', 'gen_id', 'branchdata']
def sock_init():
    for i in range(server_num):
        tmp=socket.socket()
        tmp.connect((str(bus_ip[i][0]),int(bus_ip[i][1])+2))
        epoller.register(tmp,FLAG)
        sock.append(tmp)
        fd_socket[tmp.fileno()]=tmp
import threading
import time
recvnum=0
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
                time.sleep(3.5)
                print "ASK"
            except Exception:
                os._exit(1)
class worker2(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    def run(self):
        global recvnum
        global sock
        global busdata
        while True:
            try:
                time.sleep(0.1)
                if recvnum==server_num:
                    busdata={}
                    recvnum=0
            except Exception:
                os._exit(2)
s=worker1()
s.start()
s2=worker2()
s2.start()
def main():
    global recvnum
    global sock
    global busdata
    branchdata=[]
    while True:
        events=epoller.poll()
        for fd,flag in events:
            s=fd_socket[fd]
            if flag&(select.EPOLLIN):
               try:
                    data=s.recv(512)
                    a=np.frombuffer(data,dtype=persontype)
                    busdata=dict(zip(key,a.tolist()[0]))
                    branchdata=a['branchdata'].tolist()
                    busdata['branchdata']=branchdata[0][0:a['branchnum']]
                    update_bus_data(int(busdata['bus_id']),busdata)
                    recvnum=recvnum+1
               except Exception:
                    os._exit(3)
main()

