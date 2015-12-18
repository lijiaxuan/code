from twisted.internet.protocol import Protocol,ServerFactory
from busdata import bus_data
import struct
import numpy as np
class Sensor_protocol(Protocol):
    def __init__(self):
        global bus_data
        self.bus_data = bus_data
    def dataReceived(self, data):
        if data == 'ASK':
            self.transport.write(self.bus_data.tobytes())
    def connectionMade(self):
        pass
class Sensor_Factory(ServerFactory):
    protocol = Sensor_protocol