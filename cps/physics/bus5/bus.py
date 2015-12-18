from twisted.internet.protocol import Protocol,ClientFactory
from twisted.internet import reactor
from busdata import bus_data,bustype
from smart_meter_server import ServerFactory
from actuator_server import Actuator_Factory
from sensor_server import Sensor_Factory
import struct
import numpy as np

BASEPORT = 6500

class Bus_protocol(Protocol):
    def dataReceived(self, data):
        global bus_data
        if data == 'ASK':
            self.transport.write(bus_data.tobytes())
        elif len(data) == 96:
            if bus_data['status'] == 1:
                bus_data = np.frombuffer(data,dtype=bustype)
    def connectionMade(self):
        pass
class Bus_Factory(ClientFactory):
    protocol = Bus_protocol
reactor.listenTCP(BASEPORT,Bus_Factory())
reactor.listenTCP(BASEPORT + 1,ServerFactory())
reactor.listenTCP(BASEPORT + 2,Sensor_Factory())
reactor.listenTCP(BASEPORT + 3,Actuator_Factory())
reactor.run()
#print len(bus_data.tobytes())