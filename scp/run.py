#!/usr/bin/env python
# ./bin/VRP -name P-n16-k8 -trucks 8 -size 50 -it 20 -path vrp/small

from subprocess import call
import os
import configs
import utils as ut

for Set in configs.Sets:
    for file in os.listdir(Set):
        for run in range(configs.RUNS):
            for popSize in configs.SIZE:
                for maxIt in configs.MAX_IT:
                    fileArray = os.path.splitext(file)
                    if(fileArray[1] == ".vrp"):
                        if not os.path.exists(configs.logDir+fileArray[0]):
                            os.makedirs(configs.logDir+fileArray[0])
                        
                        print("Starting "+ str(run) +" run of "+fileArray[0])

                        call(["./bin/VRP", "-name "+fileArray[0], "-trucks "+ut.getTrucks(file), "-size "+popSize, "-it "+maxIt, "-path "+str(Set), "-opt "+str(ut.getBest(file, Set)), "-log "+configs.logMode, "-run "+str(run)])

'''
import random

vrps=[]
aux=[]
for dirSet in os.listdir("vrp"):
    for vrp in os.listdir("vrp/"+dirSet):
        print(vrp)
        aux.append(vrp)


r = random.randint(0, len(aux))
print(aux[r])
'''