#include <iostream>
#include "GAUtils.hpp"
#include "Configs.hpp"
#include "Mutation.hpp"
#include "TourRepairer.hpp"
#include "ImportData.hpp"

#include "InitialPop.hpp"
#include "Crossover.hpp"

Population newGeneration(Population& pop){
    Crossover::crossoverPopulation(pop);
    for(auto tour:pop.getPop()){
            tour=TourRepairer().repairTour(tour);
    }
    pop.sortPop();
    applyMutation(pop);  
    popReset(pop);
    return(pop);
}
void popReset(Population &pop){
    int nToKeep =(int) (double)(pop.getPop().size()) * (double)(ResetConfigs::nBestToKeep/(double)100);
    for(unsigned i=(nToKeep+1);i<pop.getPop().size();i++){
        for(unsigned j=0;j<ResetConfigs::resetMutIterations;j++){
            pop.getPop()[i]=InitialPop::tourGen();
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
            pop.getPop()[i]=Mutation().evaluateMutation(pop.getPop()[i].getRoute());
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
        }
    }
    pop.sortPop();
}
void applyMutation(Population &pop){
    double Rate=(double)MutationCtrl::mutationRate/(double)100;
    Mutation mut;
    //std::cout<<"Rate::"<<Rate<<std::endl;
    unsigned numIndiv=pop.getPop().size()*Rate;
    //std::cout<<"Num Indiv Mutate: "<<numIndiv<<std::endl;
    if(MutationCtrl::applyWorst){
        for(unsigned i=pop.getPop().size()-1;i>(pop.getPop().size()-(numIndiv));i--){
            pop.getPop()[i]=Tour(mut.evaluateMutation(pop.getPop()[i].getRoute()));
             pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);  
        }
    }else{
        for(unsigned i=0;i<numIndiv;i++){
            pop.getPop()[i]=Tour(mut.evaluateMutation(pop.getPop()[i].getRoute()));
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);  
        }  
    }
    pop.sortPop();
}

/* Run control */
void RunControl::initAlg(Population& pop) {
    //Setting the data
    ImportData vrpFile(Configs::pathToFile+Configs::file);
    Globals::customerMap = CustomerMap(vrpFile.getCustomerList(), vrpFile.getCapacity());
    //Generates a random pop and applies mutation
    pop = InitialPop::popGen(Configs::popSize);
    InitialPop::initialPopApplyMutation(pop);
    Fitness::initialBest=pop.getBestSolution().second;
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