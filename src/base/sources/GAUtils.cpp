#include <iostream>
#include "GAUtils.hpp"
#include "Configs.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"
#include "HamiltonianCycle.hpp"
#include "TourRepairer.hpp"
#include "ImportData.hpp"

vector<int> tourGen(){
    vector<int> tour;
    int depotId=Globals::customerMap.getDepotId();

        for(auto c : Globals::customerMap.getMap()){
            if(c.getId() != depotId){
                tour.push_back(c.getId());
            }
        }
        std::shuffle(tour.begin(), tour.end(), Globals::urng);

        unsigned tamGambi = tour.size() + Configs::truckNumber;
        int i=0, backsGap = (tour.size()+1)/Configs::truckNumber;
        while(tour.size() != tamGambi){
            tour.insert(tour.begin()+i, depotId);
            i+=backsGap+1;
        }
       return tour;
}

Population popGen(int popSize){
    Population pop;
    for(int x=0; x<popSize; x++){
        Tour tour= Tour(tourGen());
        pop.addNewTour(tour);
    }
    return (pop);
}

void initialPopApplyMutation(Population &pop){
    double Rate= (double)MutationCtrl::InitialPopMutRate/(double)100;
    unsigned numIndivs=(pop.getPop().size()*Rate);
    //std::cout <<"Rate:"<<Rate<<std::endl;
    //std::cout <<"Num Indiv Mutate: "<<  numIndivs<<std::endl;
    for(unsigned i=0;i<MutationCtrl::InitialPopmutIterations;i++){
        for(unsigned i=0;i<numIndivs;i++){
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
        }
        for(unsigned i=0;i<numIndivs;i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation((pop.getPop()[i].getRoute())));
        }
        for(unsigned i=0;i<numIndivs;i++){
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
        }
        
    }
    pop.sortPop();
}
void applyMutation(Population &pop){
    double Rate=(double)MutationCtrl::mutationRate/(double)100;
    
    //std::cout<<"Rate::"<<Rate<<std::endl;
    unsigned numIndiv=pop.getPop().size()*Rate;
    //std::cout<<"Num Indiv Mutate: "<<numIndiv<<std::endl;
    if(MutationCtrl::applyWorst){
        for(unsigned i=pop.getPop().size()-1;i>(pop.getPop().size()-(numIndiv));i--){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation(pop.getPop()[i].getRoute()));
             pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);  
        }
    }else{
        for(unsigned i=0;i<numIndiv;i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation(pop.getPop()[i].getRoute()));
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);  
        }  
    }
}
void popReset(Population &pop){
    int nToKeep =(int) (double)(pop.getPop().size()) * (double)(ResetConfigs::nBestToKeep/(double)100);
    for(unsigned i=(nToKeep+1);i<pop.getPop().size();i++){
        for(unsigned j=0;j<ResetConfigs::resetMutIterations;j++){
            pop.getPop()[i]=tourGen();
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
            pop.getPop()[i]=Mutation().evaluateMutation(pop.getPop()[i].getRoute());
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
        }
    }
    pop.sortPop();
}
Population newGeneration(Population& pop){
    crossoverPopulation(pop);
    pop.sortPop();
    for(auto tour:pop.getPop()){
            tour=TourRepairer().repairTour(tour);
    }
    pop.sortPop();
    applyMutation(pop);  
    pop.sortPop();
    return(pop);
}

Population crossoverPopulation(Population& pop){
    unsigned size{(unsigned)pop.getPop().size()};
    Population aux;

    for(unsigned i=0; i<size; i++){
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
    }
    return(aux);
}

Tour crossover(Tour& red, Tour& blue){
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    return(offspring);
}


/* Run control */
void RunControl::initAlg(Population& pop) {
    ImportData vrpFile(Configs::pathToFile+Configs::file);

    Globals::customerMap = CustomerMap(vrpFile.getCustomerList(), vrpFile.getCapacity());

    pop = popGen(Configs::popSize);

    initialPopApplyMutation(pop);
    Fitness::initialBest=pop.getPop()[0].getDist();
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