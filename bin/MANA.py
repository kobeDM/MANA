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
MANAVIEW="MANA_view"
MANASPEC="MANA_spec"
#VIEW_SKEL="View_skel.cc"
CONFIG="MADA_config.json"

print("**MANA.py**")
print("**in Micacle argon ANAlysis (http://github.com/kobeDM/MADA)**")
print("**2021 Sep by K. Miuchi**")
files=['GBIP_016_0000.raw','GBIP_024_0000.raw']

for file in files:
    cmd='xterm -e '+MANAPATH+MANASPEC+" "+file
    #file="GBIP_016_0000.raw"
    #cmd=MANAPATH+MANASPEC+" "+file
    print(cmd)
    proc=subprocess.Popen(cmd,shell=True)
