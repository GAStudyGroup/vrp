#!/usr/bin/env python
import sys

try:
    file = open("log.txt","r")
except OSError:
    pass

i=0
for line in file:
    i+=1
    if line.find("CROSSOVER ")!=-1:
        array = line.split(" ")
        if (float(array[3]) < float(array[1])) and (float(array[3]) < float(array[2])):
            print(line)