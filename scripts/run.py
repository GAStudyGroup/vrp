#!/usr/bin/env python
from subprocess import call
import os

population_size = 50
truck_number = 8
mutation_iterations = 10
disturb_factor = 10
elitism_percentage = 10

tours = ["libs/eil51.vrp"]

for tour_name in tours:
    for i in range(1):

        print("\n\nStarting GA\n")

        #chamar o GA
        call(["./bin/GA", tour_name, str(truck_number), str(population_size), str(mutation_iterations), str(disturb_factor), str(elitism_percentage)])

        '''
        print("Validate crossover")
        call(["./validate_crossover.py", str(i), str(tour_name)])
        print("end validate crossover")
        print("Validate build")
        call(["./validate_build.py", str(i), str(tour_name)])
        print("end validate build")
        '''