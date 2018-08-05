#ifndef CONFIGS_HPP
#define CONFIGS_HPP

#include <random>
#include <string>

#include "CustomerMap.hpp"
#include "Tour.hpp"

namespace Configs {
    //Basic Settings of GA

    //Name of file .vrp
    extern std::string file; 
    //Path to the folder where the file is (if have)
    extern std::string pathToFile;
    //Truck number to be used
    extern unsigned truckNumber;
    //Size of the population
    extern unsigned popSize;
    //Fitness mode, value must be 0 = OX or 1 = GPX
    extern int fitnessMode;
    //Run id, used to Log
    extern unsigned runId;
    //Type of crossover, 1 or 2
    extern unsigned crossoverType;    
    //Max number of iterations
    extern unsigned maxIterations;
    //Best known solution
    extern unsigned optimalValue;
    //Method of write log
    extern unsigned logMethod;

    //In case of OX
    extern unsigned elitismNumber;

    extern unsigned initialPopMethod;
    extern bool withMutation;
}

namespace MutationCtrl {
    
    //Mutation improvements settings
    //Store tours that can't be improved anymore
    extern vector<Tour> mutationMaxedList;

    //Initial population settings
    //Quantity of iterations to be realized on the initial population
    extern unsigned InitialPopmutIterations;
    //Percentage of initial population that will receive mutation
    extern unsigned InitialPopMutRate;
    
    //Loop Mutation settings
    //Amount of populations that will receive mutation
    extern unsigned mutationRate;
    //true applies mutation to the best, false applies mutation to the worst
    extern bool applyWorst;     
}

namespace ResetConfigs {
    //Reset Settings
    //Maintain the n best tours during reset
    extern int nBestToKeep;
    //Defines how many iterations of mutation will be applied during reset
    extern unsigned resetMutIterations;
}

namespace Fitness {
    //Fitness params
    //Best of the first pop
    extern double initialBest;
}

namespace KmeansCfg{
    //Kmeans param
    //Kmeans max iterations, if set equal -1 it will run until there is no more centroids changes
    //if set to a larger value it will run the especified number of iterations
    extern int KmeansIterations;
}

namespace Globals {
    extern CustomerMap customerMap;
    extern std::mt19937 urng;
    //Iteration counter
    extern unsigned currentIteration;

    extern std::ofstream* debugLogFile;
}

#endif