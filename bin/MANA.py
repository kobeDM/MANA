#!/usr/bin/python3

import os, sys
import subprocess
import numpy
import glob
import time
import argparse
import json

from subprocess import PIPE
#from subprocess import STDOUT
HOME=os.environ["HOME"]
MANAPATH=HOME+"/miraclue/MANA/bin/"
MANAVIEW="MANA_view"
MANASPEC="MANA_spec"
#VIEW_SKEL="View_skel.cc"
CONFIG="MADA_config.json"

print("**MANA.py**")
print("**in Micacle argon ANAlysis (http://github.com/kobeDM/MADA)**")
print("**2021 Sep by K. Miuchi**")
files=[]
dirs=[]


parser = argparse.ArgumentParser()
parser.add_argument("-s",help="start file ID",default=0,type=int)
parser.add_argument("-e",help="end file ID",default=1000,type=int)
args = parser.parse_args( )
start=args.s
end=args.e

fileid=start

#load config file
config_open= open(CONFIG,'r')
config_load = json.load(config_open)
#activeIP=[]
for x in config_load['gigaIwaki']:
#    files.append(x+"_0000.mada")
    files.append(x+"_"+(str(fileid).zfill(4))+".mada")
    dirs.append(os.getcwd());

#    if config_load['gigaIwaki'][x]['active']==1:
#        activeIP.append(config_load['gigaIwaki'][x]['IP'])
#        print(config_load['gigaIwaki'][x]['IP'])
#load config file ends.

print(files)

for i in range(len(files)):
#for file in files:
    cmd='xterm -geometry 50x5+300+'+str(i*100)+' -e '+MANAPATH+MANASPEC+" "+files[i]+" "+dirs[i]
    #file="GBIP_016_0000.raw"
    #cmd=MANAPATH+MANASPEC+" "+file
    print(cmd)
    proc=subprocess.Popen(cmd,shell=True)
