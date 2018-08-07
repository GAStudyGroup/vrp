#!/usr/bin/env python

from subprocess import call
import os
import testConfigs as cfg
import utils as ut

for DIR, INSTANCES in cfg.DATASETS.items():
    for instance in INSTANCES:
        for run in range(cfg.TOTAL_RUNS):
            for popSize in cfg.POP_SIZE:
                for maxIt in cfg.MAX_IT:
                    for has_mut in cfg.WITH_MUTATION:
                        for cross in cfg.CROSS:
                            for in_method in cfg.INITIAL_METHOD:

                                if not os.path.exists(cfg.LOG_DIR+instance):
                                    os.makedirs(cfg.LOG_DIR+instance)

                                print("Starting "+str(run)+" run of "+instance+" with cross "+str(cross))

                                trucks = ut.getTrucks(instance)
                                best = ut.getBest(instance, "vrp/"+DIR)
                                call(["./bin/VRP", "-name "+instance, "-trucks "+str(trucks), "-size "+str(popSize), "-it "+str(maxIt), "-path vrp/"+DIR, "-log "+str(cfg.LOG_MODE), "-run "+str(run), "-cx "+str(cross), "-inmethod "+str(in_method), "-mutation "+str(has_mut), "-opt "+str(best)])
