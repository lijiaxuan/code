from busdata import bus_data,bus_ip
import socket
import struct
def shutdown_bus(bus_data):
    if bus_data['status'] == 0:
        return
    bus_data['status']=0
    bus_data['pd']=0
    bus_data['pg']=0
    bus_data['vm']=0
    bus_data['va']=0
    bus_data['qd']=0
    bus_data['qg']=0
    for i in range(bus_data['branchnum']):
        bus_data['branchdata'][0][i]['branch_status']=0
        bus_data['branchdata'][0][i]['q'] = 0
        bus_data['branchdata'][0][i]['p'] = 0
        if bus_data['branchdata'][0][i]['is_from'] == 1:
            tmp = bus_data['branchdata'][0][i]['bus_to']
            try:
                print tmp,bus_ip[tmp-1]
                s = socket.socket()
                s.connect((str(bus_ip[tmp-1][0]),int(bus_ip[tmp-1][1])+3))
                s.send(struct.pack('3i',2,tmp,bus_data['branchdata'][0][i]['branch_id']))
                s.close()
            except Exception:
                print 'shutdownbus socket error'
        else:
            tmp = bus_data['branchdata'][0][i]['bus_from']
            try:
                print tmp,bus_ip[tmp-1]
                s = socket.socket()
                s.connect((str(bus_ip[tmp-1][0]),int(bus_ip[tmp-1][1])+3))
                s.send(struct.pack('3i',2,tmp,bus_data['branchdata'][0][i]['branch_id']))
                s.close()
            except Exception:
                print 'shutdownbus socket error'
def start_bus(bus_data):
    if bus_data['status'] == 1:
        return
    bus_data['status']=1
def shutdown_branch(bus_data,branch_id):
    for i in range(bus_data['branchnum']):
        if bus_data['branchdata'][0][i]['branch_id'] == branch_id:
            bus_data['branchdata'][0][i]['branch_status']=0
            bus_data['branchdata'][0][i]['q'] = 0
            bus_data['branchdata'][0][i]['p'] = 0
def start_branch(bus_data,branch_id):
    if bus_data['status'] == 0:
        return
    for i in range(bus_data['branchnum']):
        if bus_data['branchdata'][0][i]['branch_id'] == branch_id:
            bus_data['branchdata'][0][i]['branch_status']=1