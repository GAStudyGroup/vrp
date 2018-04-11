#include <iostream>

#include "GAControl.hpp"
#include "Configs.hpp"
#include "ImportData.hpp"

#include "InitialPop.hpp"
#include "Crossover.hpp"
#include "Extra.hpp"

Population GenerationCtrl::newGeneration(Population& pop){
    Crossover::crossoverPopulation(pop);
    Extra::applyRepair(pop);
    Extra::applyMutation(pop);  
    Extra::popReset(pop);
    return(pop);
}

/* Run control */
void RunControl::initAlg(Population& pop) {
    //Setting the data
    ImportData vrpFile(Configs::pathToFile+Configs::file);
    Globals::customerMap = CustomerMap(vrpFile.getCustomerList(), vrpFile.getCapacity());
    //Generates a random pop and applies mutation
    pop = InitialPop::InitialPopByMutation(Configs::popSize);
    Fitness::initialBest= pop.getBestSolution().second;
}

std::ofstream RunControl::initLogFile() {
    string fileName{"log/"+Configs::file+"_run_"+ std::to_string(Configs::runId) +"_cross_"+ std::to_string(Configs::crossoverType) +".log"};

    std::ofstream logFile(fileName);
    if(!logFile.is_open()){
        std::cout << "Error in openning log File: " << fileName <<std::endl;
        exit(0);
    }
    return(logFile);
}

void RunControl::printHeader(std::ostream &out) {
    out << "Run ID: " << Configs::runId<<"\n";
    out << "Vehicle Routing Problem: " << Configs::file << "\n";
    out << "Configurations of run: \n" << "\tPopulation Size: " << Configs::popSize << "\n";
    out << "\tTruck Number: " << Configs::truckNumber << "\n\tMax Iterations: " << Configs::maxIterations << "\n";

    out << "\nBest Known solution: " << "Implementar" << std::endl;
}

void RunControl::printExecutionTime(std::ostream &out, double seconds) {
    out << "Execution time: ";
    out << "\n\t" << seconds << " seconds.";
    out << "\n\t" << ((int)(seconds/60)) << ":" << ((int)seconds%60) << " minutes.";
    out << std::endl;
}

bool RunControl::stopAlg(Population& pop) {
    // Colocar para validar o total a bestSolution com python
    if(Globals::currentIteration < Configs::maxIterations) {
        return(true);
    } else {
        return(false);
    }
}