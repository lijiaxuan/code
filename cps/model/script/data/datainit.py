__author__ = 'sapphire'
import mpc_data
import numpy as np
persontype =np.dtype([('bus_id','<i2'),('type','i1'),('status','i1'),('pg','<f4')
    ,('qg','<f4'),('pd','<f4'),('qd','<f4'),('vm','<f4'),('va','<f4'),('branchnum','i1'),('al','i1'),('gen_id','<i2')
    ,('branchdata',[('is_from','i1'),('branch_status','i1'),('bus_from','<i2'),('bus_to','<i2'),('branch_id','<i2'),('p','<f4'),('q','<f4')],4)],align=True)
iptype = np.dtype([('ip','S15'),('port','<i2')])
list=[(mpc_data.mpc_pure_bus[0],mpc_data.mpc_pure_bus[1],1,0,0,0,0,0,0,3,)]
bus_branch=[]
def getbranch(id):
    branch_data=[]
    tmp=[]
    for i,item in enumerate(mpc_data.mpc_pure_branch):
        if item[0]==int(id+1):
            branch_data.append((1,1,item[0],item[1],i,0,0))
            tmp.append([1,1,item[0],item[1],i])
        if item[1]==int(id+1):
            branch_data.append((0,1,item[0],item[1],i,0,0))
            tmp.append([0,1,item[0],item[1],i])
    bus_branch.append(tmp)
    s=len(branch_data)
    for i in range(4-s):
        branch_data.append((0,0,0,0,0,0,0))
    return [s,branch_data]
data_init=[]
gen_id={1:0,2:1,3:2}
pd=[]
for i in range(9):
    branch=getbranch(i)
    try:
        _id=gen_id[i+1]
    except:
        _id=0xff
    if(mpc_data.mpc_pure_bus[i][2]!=0):
        pd.append([mpc_data.mpc_pure_bus[i][2],mpc_data.mpc_pure_bus[i][3]])
        data_init.append((mpc_data.mpc_pure_bus[i][0],mpc_data.mpc_pure_bus[i][1],1,0,0,mpc_data.mpc_pure_bus[i][2],mpc_data.mpc_pure_bus[i][3],0,0,branch[0],0,_id,branch[1]))
    else:
        pd.append([30,30])
        data_init.append((mpc_data.mpc_pure_bus[i][0],mpc_data.mpc_pure_bus[i][1],1,0,0,30,30,0,0,branch[0],0,_id,branch[1]))
for i in range(len(data_init)):
    ss=np.array(data_init[i],dtype=persontype)
    ss.tofile("/home/obsidian/program/ljx/security/cps/physics/bus"+str(i+1)+"/config")
ip = [
	('192.168.3.32',	6100),
	('192.168.3.31', 	6200),
	('192.168.3.34',	6300),
	('192.168.3.36',	6400),
	('192.168.3.40',	6500),
	('192.168.3.41',	6600),
	('192.168.3.43',	6700),
	('192.168.3.45',	6800),
	('192.168.3.46',	6900),
	]
for i in range(9):
    x = np.array(ip,dtype=iptype)
    x.tofile("/home/obsidian/program/ljx/security/cps/physics/bus"+str(i+1)+"/ip")
x.tofile("/home/obsidian/program/ljx/security/cps/model/model/sensor/ip")
x.tofile("/home/obsidian/program/ljx/security/cps/model/model/smartmeter/ip")
for i,item in enumerate(mpc_data.mpc_pure_gen):
    gen_id[item[0]]=i
for i in range(len(mpc_data.mpc_pure_bus)):
    getbranch(i)