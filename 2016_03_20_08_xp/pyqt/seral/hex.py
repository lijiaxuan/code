import binascii
import struct
import serial
import numpy as np
import socket
import time
datatype = np.dtype([('x','<f'),('y','<f')])
comm = serial.Serial(2,baudrate=115200)
text = ''

HOST='127.0.0.1'
PORT=50007
s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect((HOST,PORT))

while 1:
     num2read1 =  comm.inWaiting()
     if num2read1 != 0:
         data = comm.read(num2read1)
         text = text + data
         for i in text.split(struct.pack('B',0xff)):
             try:
                 if len(i) == 0:
                     continue
                 arr = np.frombuffer(i,dtype=datatype)
                 s.sendall(i)
             except Exception:
                 text = i
                 break