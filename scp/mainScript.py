#!/usr/bin/env python
import subprocess
import os

pythonRun = "scp/testUtfRun.py"
actualDir = os.path.dirname(os.path.abspath(__file__))

subprocess.call(["chmod", "+x", pythonRun])

subprocess.Popen(["konsole", "--noclose", "--workdir", ".", "-e", "./"+pythonRun, "0"])
subprocess.Popen(["konsole", "--noclose", "--workdir", ".", "-e", "./"+pythonRun, "1"])
subprocess.Popen(["konsole", "--noclose", "--workdir", ".", "-e", "./"+pythonRun, "2"])
subprocess.Popen(["konsole", "--noclose", "--workdir", ".", "-e", "./"+pythonRun, "3"])