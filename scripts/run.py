#!/usr/bin/env python
from subprocess import call
import os

tours = ["libs/A-Sets/A-n32-k5", "libs/A-Sets/A-n63-k10", "libs/A-Sets/A-n80-k10", "libs/E-Sets/E-n101-k14", "libs/E-Sets/E-n101-k8", "libs/P-Sets/P-n16-k8", "libs/P-Sets/P-n50-k8", "libs/P-Sets/P-n50-k8", "libs/P-Sets/P-n51-k10"]

for tour_name in tours:
    for i in range(1):

        print("\n\nStarting GA\n")

        #chamar o GA
        name = tour_name.split("/")[2]

        f=open("Logs/"+name+".log", "w")
        call(["./bin/GA", tour_name+".vrp"], stdout=f)

        '''
        print("Validate crossover")
        call(["./validate_crossover.py", str(i), str(tour_name)])
        print("end validate crossover")
        print("Validate build")
        call(["./validate_build.py", str(i), str(tour_name)])
        print("end validate build")
        '''