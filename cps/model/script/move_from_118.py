__author__ = 'sapphire'
import commands
for i in range(118):
    try:
        print commands.getoutput("cp -r /home/sapphire/program/code/chen_test/distributedir/physical"+str(i+1)+"/config/config.json ../physics/physic"+str(i+1)+"/config")
        print commands.getoutput("cp -r /home/sapphire/program/code/chen_test/distributedir/physical"+str(i+1)+"/config/gen.json ../physics/physic"+str(i+1)+"/config")
    except:
        pass