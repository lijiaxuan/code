__author__ = 'sapphire'
import struct
import socket

print "192.168.11."+str(100)
for i in range(1):
    sock=socket.socket()
    sock.connect(("192.168.11."+str(i+100),6000))
    x=struct.pack('3i',i+1,2,0)
    sock.send(x)
    sock.close()