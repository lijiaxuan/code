import subprocess
from twisted.internet import reactor
for i in range(9):
    subprocess.Popen('python bus'+str(i+1)+'/bus.py',shell=True)
reactor.run()