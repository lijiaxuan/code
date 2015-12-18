import numpy as np
bustype = np.dtype([('bus_id','<i2'),('type','i1'),('status','i1'),('pg','<f4')
    ,('qg','<f4'),('pd','<f4'),('qd','<f4'),('vm','<f4'),('va','<f4'),('branchnum','i1'),('al','i1'),('gen_id','<i2')
    ,('branchdata',[('is_from','i1'),('branch_status','i1'),('bus_from','<i2'),('bus_to','<i2'),('branch_id','<i2'),('p','<f4'),('q','<f4')],4)],align=True)
iptype = np.dtype([('ip','S15'),('port','<i2')])
import os
BASE_PATH = os.path.dirname(os.path.abspath(__file__))
bus_data = np.fromfile(BASE_PATH+'/config',dtype=bustype)
bus_ip = np.fromfile(BASE_PATH+'/ip',dtype=iptype)