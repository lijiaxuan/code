from twisted.internet.protocol import Protocol,ServerFactory
from twisted.internet import reactor
from busdata import bus_data
import struct
from bus_action import *
class Actuator_protocol(Protocol):
    def dataReceived(self, data):
        global bus_data
        if len(data) == 12:
            command,bus_id,branch_id = struct.unpack('3i',data)
            if bus_id == bus_data['bus_id']:
                if command == 0:
                    shutdown_bus(bus_data)
                    print 'shutdownbus ',bus_data
                elif command == 1:
                    start_bus(bus_data)
                elif command == 2:
                    shutdown_branch(bus_data,branch_id)
                    print 'shutdownbranch',bus_data
                elif command == 3:
                    start_branch(bus_data,branch_id)
    def connectionMade(self):
        pass
class Actuator_Factory(ServerFactory):
    protocol = Actuator_protocol