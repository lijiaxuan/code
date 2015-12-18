__author__ = 'sapphire'
import socket
sock=socket.socket()
# sock.bind(("192.168.10.100",5030))
# sock.listen(5)
sock.connect(('',6100))
sock.send('ASK')
sock.close()
#sock.accept()