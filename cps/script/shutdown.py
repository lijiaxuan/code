import socket
import struct
_id = 2
sock = socket.socket()
sock.connect(('',6003 + _id * 100))
sock.send(struct.pack('3i',0,_id,0))
sock.close()