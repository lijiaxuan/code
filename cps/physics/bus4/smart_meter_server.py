from twisted.internet.protocol import Protocol,ServerFactory
from twisted.internet import reactor
from busdata import bus_data
import struct
import numpy as np
class SmartMeter_protocol(Protocol):
    def __init__(self):
        global bus_data
    def dataReceived(self, data):
        if len(data) == 12:
            bus_id,command,branch_id = struct.unpack('3i',data)
            if bus_id == bus_data['bus_id']:
                if command == 0:
                    pass
                elif command == 1:
                    pass
                elif command == 2:
                    pass
                elif command == 3:
                    pass
    def connectionMade(self):
        pass
class SmartMeter_Factory(ServerFactory):
    protocol = SmartMeter_protocol