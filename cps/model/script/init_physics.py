__author__ = 'sapphire'
import commands
for i in range(118):
    try:
        print commands.getoutput("ifconfig eth0:"+str(i)+" 192.168.10."+str(i+100)+" up")
    except:
        pass
for i in range(118):
    try:
        print commands.getoutput("ifconfig eth0:"+str(i+118)+" 192.168.11."+str(i+100)+" up")
    except:
        pass