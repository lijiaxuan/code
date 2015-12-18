__author__ = 'sapphire'
from twisted.internet import reactor
from twisted.internet.protocol import Protocol, ClientFactory
import struct
# def checksum(data):
#     length = len(data)
#     s = 0
#     n = length % 2
#     for i in range(0, length - n, 2):
#         s += ord(data[i]) + (ord(data[i+1]) << 8)
#     if n:
#         s += ord(data[-1])
#     while (s >> 16):
#         s = (s & 0xFFFF) + (s >> 16)
#     s = ~s & 0xffff
#     return s
# import numpy as np
# persontype =np.dtype([('bus_id','<i2'),('type','i1'),('status','i1'),('pg','<f4')
#     ,('qg','<f4'),('pd','<f4'),('qd','<f4'),('vm','<f4'),('va','<f4'),('branchnum','i1'),('al','i1'),('gen_id','<i2')
#     ,('branchdata',[('is_from','i1'),('branch_status','i1'),('bus_from','<i2'),('bus_to','<i2'),('branch_id','<i2'),('p','<f4'),('q','<f4')],15)],align=True)
# a=np.array([(1,2,1,3.3,5.3,4.3,532.2,123.3,54.1,0,[0,0,0],[(2,0,3,4,[0,0],6.3,3.2)for i in range(5)])],dtype=persontype)
# # checksum(a.tobytes())
# print a['bus_id'][0]
# # print struct.pack('H',checksum(a.tobytes()))
# a.tofile("/home/sapphire/program/code/c++/physics/test/np")
# print np.fromfile("/home/sapphire/program/code/c++/physics/test/testa2",dtype=persontype)
class protocol(Protocol):

    """protocol"""

    def dataReceived(self, data):
        print "data is",data
        # print "len is ",len(data)
        # x=struct.pack('H',checksum(a.tobytes()))
        # self.transport.write(a.tobytes()+x)
        # print "array",np.frombuffer(data[0:len(data)-2],dtype=persontype)
    def connectionMade(self):
        print "connectionMade"
        self.transport.write("ASK")

class fac(ClientFactory):

    """docstring for fac"""
    protocol = protocol
print "here"


def loop():
    reactor.connectTCP("",6000, fac())
    #reactor.connectTCP("",6002, fac())
    reactor.callLater(2, loop)
    print "loop"
loop()
reactor.run()