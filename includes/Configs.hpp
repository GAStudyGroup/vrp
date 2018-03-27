#ifndef CONFIGS_HPP
#define CONFIGS_HPP
#include "CustomerMap.hpp"
#include "Tour.hpp"
namespace Configs {
    extern unsigned truckNumber;
    extern unsigned popSize;
    extern unsigned disturbFactor;
    extern unsigned elitismNumber;
    extern unsigned currentIteration;
    extern unsigned maxIterations;
    extern int fitnessMode;
    extern CustomerMap customerMap;
    extern double initialBest;

    //Initial population settings
    extern unsigned InitialPopmutIterations;
    extern unsigned InitialPopMutRate;
    
    //Loop Mutation settings
    extern unsigned mutationRate;
    extern bool applyWorst; //true applies mutation to the best, false applies mutation to the worst

    //Mutation improvements settings
    extern vector<Tour> mutationMaxedList;
}

#endif