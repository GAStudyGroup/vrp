#!/usr/bin/env python
from subprocess import call
import os
import configs
import utils as ut

for file in os.listdir(configs.PATH+configs.Psets):
    for run in range(configs.RUNS):
        for popSize in configs.SIZE:
            for maxIt in configs.MAX_IT:
                print("Starting "+ str(run) +" run of "+file)

                call(["./bin/VRP", "-name "+file, "-trucks "+ut.getTrucks(file), "-size "+popSize, "-it "+maxIt, "-path "+str(configs.PATH+configs.Psets)])
'''
for tour_name in tours:
    for i in range(1):

        print("\n\nStarting GA\n")

        #chamar o GA
        name = tour_name.split("/")[2]

        f=open("logs/"+name+".log", "w")
        call(["./bin/GA", tour_name+".vrp"], stdout=f)


from subprocess import call
import os
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