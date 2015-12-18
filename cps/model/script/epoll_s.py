__author__ = 'sapphire'
import select
import socket
import time
server_num=1
sock=[]
FLAG=select.EPOLLET|select.EPOLLIN
epoller=select.epoll()
fd_socket={}
import numpy as np
persontype =np.dtype([('bus_id','<i2'),('type','i1'),('status','i1'),('pg','<f4')
    ,('qg','<f4'),('pd','<f4'),('qd','<f4'),('vm','<f4'),('va','<f4'),('branchnum','i1'),('al','i1'),('gen_id','<i2')
    ,('branchdata',[('is_from','i1'),('branch_status','i1'),('bus_from','<i2'),('bus_to','<i2'),('branch_id','<i2'),('p','<f4'),('q','<f4')],15)],align=True)
a=np.array([(1,2,1,3.3,5.3,4.3,532.2,123.3,54.1,0,1,2,[(2,0,3,4,9,6.3,3.2)for i in range(15)])],dtype=persontype)
#a=np.zeros(1,dtype=persontype)
x=a.tobytes()
import struct
m=struct.pack('2b',0x43,0x53)
def sock_init():
    for i in range(server_num):
        tmp=socket.socket()
        tmp.connect(("",i+6000))
        epoller.register(tmp,FLAG)
        sock.append(tmp)
        fd_socket[tmp.fileno()]=tmp
import threading
import time
recvnum=0
class worker(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        sock_init()
    def run(self):
        global recvnum
        while True:
            recvnum=0
            for i in sock:
                i.send("ASK")
                pass
            time.sleep(2)
            print "ASK"
s=worker()
s.start()
def main():
    global recvnum
    while True:
        events=epoller.poll()
        for fd,flag in events:
            s=fd_socket[fd]
            if flag&(select.EPOLLIN):
                data=s.recv(512)
                recvnum=recvnum+1
                if recvnum==server_num:
                    print "recv over"

                print s.send(x+m)
main()

