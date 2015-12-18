__author__ = 'sapphire'
#coding=utf-8
'''
数据处理,将得到的信息写回case39.m
'''
from mpc_data import *
from caculate import runmat
import os
import numpy as np
import struct
persontype =np.dtype([('bus_id','<i2'),('type','i1'),('status','i1'),('pg','<f4')
    ,('qg','<f4'),('pd','<f4'),('qd','<f4'),('vm','<f4'),('va','<f4'),('branchnum','i1'),('al','i1'),('gen_id','<i2')
    ,('branchdata',[('is_from','i1'),('branch_status','i1'),('bus_from','<i2'),('bus_to','<i2'),('branch_id','<i2'),('p','<f4'),('q','<f4')],4)],align=True)
base_path=os.path.dirname(os.path.abspath(__file__))
base_path=base_path.replace("cps/model/model","cps/model/")
class data_ctler():
    def __init__(self,filepath=base_path+"matpower/"):
        self.filepath=filepath
#####################################
    def write_bus(self,bus,f):
        '''
        将bus信息写入case9文件
        bus_i	type	Pd	Qd	Gs	Bs	area	Vm	Va	baseKV	zone	Vmax	Vmin
        :param bus:[[1,2,3,1,2],[2,1,2,3,2]]
        :f 文件对象
        :return:
        '''
        f.writelines("mpc.bus =")
        tmp=str(bus).replace(']]','];').replace('[[','[').replace('], [',';\n\t').replace(',','\t').replace(' ','').replace('];',';\n];')
        f.writelines('[\n\t')
        f.writelines(tmp[1:]+'\n')
    def write_gencost(self,gencost,f):
        '''
         将gencost信息写入case9文件
         1	startup	shutdown	n	x1	y1	...	xn	yn
        :param gencost: [[1,2,3,1,2],[2,1,2,3,2]]
        :param f: 文件对象
        :return:
        '''
        f.writelines("mpc.gencost = ")
        tmp=str(gencost).replace(']]','];').replace('[[','[').replace('], [',';\n\t').replace(',','\t').replace(' ','').replace('];',';\n];')
        f.writelines('[\n\t')
        f.writelines(tmp[1:]+'\n')
    def write_gen(self,gen,f):
        '''
         将gen信息写入case9文件
         [bus	Pg	Qg	Qmax	Qmin
         Vg	mBase	status	Pmax	Pmin
        Pc1	Pc2	Qc1min	Qc1max	Qc2min
        Qc2max	ramp_agc	ramp_10
        ramp_30	ramp_q	apf]
        :param gen:
        :param f:
        :return:
        '''
        f.writelines("mpc.gen = ")
        tmp=str(gen).replace(']]','];').replace('[[','[').replace('], [',';\n\t').replace(',','\t').replace(' ','').replace('];',';\n];')
        f.writelines('[\n\t')
        f.writelines(tmp[1:]+'\n')
    def write_branch(self,branch,f):
        '''
        将gen信息写入case9文件
        [fbus	tbus	r	x	b	rateA	rateB	rateC	ratio
        angle	status	angmin	angmax]
        :return:
        '''
        f.writelines("mpc.branch =")
        tmp=str(branch).replace(']]','];').replace('[[','[').replace('], [',';\n\t').replace(',','\t').replace(' ','').replace('];',';\n];')
        f.writelines('[\n\t')
        f.writelines(tmp[1:]+'\n')
    def write_other(self,f):
        '''
        将case9.m的其他信息写入文件中
        :param f:
        :return:
        '''
        f.writelines("function mpc = case9Q\n")
        f.writelines("mpc.version = '2';\n")
        f.writelines("mpc.baseMVA = 100;\n")
        #f.writelines("mpc.areas = [ 1    5;];\n")
    def write_file(self):
        '''
        将信息写入case9.m中
        调用函数
        write_bus
        write_gen
        write_gencost
        write_branch
        :return:
        '''
        #try:
        f=open(self.filepath+'case9Q.m','w')
        self.write_other(f)
        self.write_bus(mpc_pure_bus,f)
        self.write_gen(mpc_pure_gen,f)
        self.write_branch(mpc_pure_branch,f)
        self.write_gencost(mpc_pure_gencost,f)
        f.close()
        #except:
        #    print "open error"
    def checksum(self,data):
        length = len(data)
        s = 0
        n = length % 2
        for i in range(0, length - n, 2):
            s += ord(data[i]) + (ord(data[i+1]) << 8)
        if n:
            s += ord(data[-1])
        while (s >> 16):
            s = (s & 0xFFFF) + (s >> 16)
        s = ~s & 0xffff
        return s
    def find_data(self,index,data):
        '''
        data:[bus,gen,branch]
        从输出中找到要发送的代码
        tmp=[1,2,1,1,1]
        :return:
        '''
        branch_data=[]
        for item in bus_branch[index-1]:#item[4] branchid
            if item[0]==1: #from
                branch_data.append((1,mpc_pure_branch[item[4]][10],item[2],item[3],item[4],data[2][item[4]][13],data[2][item[4]][14]))
            if item[0]==0: #to
                branch_data.append((0,mpc_pure_branch[item[4]][10],item[2],item[3],item[4],data[2][item[4]][15],data[2][item[4]][16]))
        s=len(branch_data)
        for i in range(4-s):
            branch_data.append((0,0,0,0,0,0,0))
        if mpc_pure_bus[index-1][1]==4:
            try:
                g_id=gen_id[mpc_pure_bus[index-1][0]]
                x=np.array([(mpc_pure_bus[index-1][0],3,0,0,0,0,0,0,0
                    ,len(bus_branch[index-1]),0,g_id,branch_data)],dtype=persontype)
            except:
                x=np.array([(mpc_pure_bus[index-1][0],1,0,0,0,0,0,0,0
                    ,len(bus_branch[index-1]),0,0xff,branch_data)],dtype=persontype)
            return x.tobytes()#+struct.pack('H',self.checksum(x.tobytes()))
        if mpc_pure_bus[index-1][1]==2 or mpc_pure_bus[index-1][1]==3: #type
            x=np.array([(mpc_pure_bus[index-1][0],mpc_pure_bus[index-1][1],1,data[1][gen_id[index]][1],data[1][gen_id[index]][2],0,0,data[0][index-1][7],data[0][index-1][8]
                ,len(bus_branch[index-1]),0,gen_id[index],branch_data)],dtype=persontype)
            return x.tobytes()#+struct.pack('H',self.checksum(x.tobytes()))
        elif mpc_pure_bus[index-1][1]==1:
            x=np.array([(mpc_pure_bus[index-1][0],mpc_pure_bus[index-1][1],1,0,0,mpc_pure_bus[index-1][2],mpc_pure_bus[index-1][3],data[0][index-1][7],data[0][index-1][8]
            ,len(bus_branch[index-1]),0,0xff,branch_data)],dtype=persontype)
            return x.tobytes()#+struct.pack('H',self.checksum(x.tobytes()))
class update_matrix():
    def __init__(self):
        pass
    def update_matrix(self,data):
        '''
        将上传来的数据写入mpc_data中,再写入case9文件
        :param queue:
        :return:
        '''
        arr=np.frombuffer(data[0:len(data)],dtype=persontype)
        bus_index=arr['bus_id'][0]-1
        if arr['status'][0]==0:
            #print arr
            mpc_pure_bus[bus_index][1]=4 #type=4
        elif arr['type'][0]==2 or arr['type'][0]==3:
            mpc_pure_gen[arr['gen_id'][0]][1]=arr['pg'][0]
            mpc_pure_gen[arr['gen_id'][0]][2]=arr['qg'][0]
            mpc_pure_bus[bus_index][7]=arr['vm'][0]
            mpc_pure_bus[bus_index][8]=arr['va'][0]
            mpc_pure_bus[bus_index][1]=3
        elif arr['type'][0]==1:
            mpc_pure_bus[bus_index][2]=arr['pd'][0]
            mpc_pure_bus[bus_index][3]=arr['qd'][0]
            mpc_pure_bus[bus_index][7]=arr['vm'][0]
            mpc_pure_bus[bus_index][8]=arr['va'][0]
            mpc_pure_bus[bus_index][1]=1
        for i in range(arr['branchnum']):
            mpc_pure_branch[arr['branchdata'][0][i][4]][10]=arr['branchdata'][0][i]['branch_status']

class caculater():
    '''
    开始计算
    '''
    def __init__(self,filepath=base_path+'matpower/'):
        self.data_ctler=data_ctler(filepath)
    def cal(self):
        '''
        计算case9 返回[bus,gen]
        :return:
        '''
        self.data_ctler.write_file()
        return runmat()
# s=data_ctler()
# ca=caculater()
# for i in range(118):
#     s.find_data(i,data)