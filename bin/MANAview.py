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

MANAPATH="/home/msgc/miraclue/MANA/bin/"
VIEW="MANA_view"
#VIEW_SKEL="View_skel.cc"
CONFIG="MADA_config.json"

print("**MANAView.py**")
print("**in Micacle argon ANAlysis (http://github.com/kobeDM/MADA)**")
print("**2021 Sep by K. Miuchi**")

# make config file from skelton file
#VIEW_SKEL_FULL=MANAPATH+VIEW

files=['GBIP_016_0000.raw','GBIP_024_0000.raw']
#print("\tMANA four boards viewer macro.")
for file in files:
    cmd='xterm -e '+MANAPATH+VIEW+" "+file
    print(cmd)
    proc=subprocess.Popen(cmd,shell=True)
#    print(file)
#~/miraclue/MANA/bin/MANAView GBIP_016_0000.raw'

#
#, env=dict(os.environ, DISPLAY=":0.0", XAUTHORITY="/home/msgc/.Xauthority"))

#cmd='xterm -e ~/miraclue/MANA/bin/MANAView GBIP_024_0000.raw'
#print(cmd)
#proc=subprocess.Popen(cmd,shell=True,stdout=PIPE,stderr=None)

