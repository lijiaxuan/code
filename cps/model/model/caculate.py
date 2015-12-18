__author__ = 'sapphire'
from oct2py import Oct2Py
oc=Oct2Py()
import os
base_path=os.path.dirname(os.path.abspath(__file__))
base_path=base_path.replace("cps/model/model","cps/model/matpower")
def runmat():
    oc.addpath(base_path)
    [baseMVA, bus, gen, gencost, branch, f, success, et]=oc.runopf("case9Q.m")
    return [bus,gen,branch]