#include <iostream>
#include "GAUtils.hpp"
#include "Configs.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"
#include "HamiltonianCycle.hpp"
#include "TourRepairer.hpp"

using std::cout;
using std::endl;

bool validateTour(vector<int> tour){
    std::sort(tour.begin(),  tour.end());
    vector<int>::iterator it;

    it=adjacent_find(tour.begin(), tour.end());

    while(1){
        if(it==tour.end()){
            break;
        } else {
            if(*it != Configs::customerMap.getDepotId()){
                break;
            }else{
                //cout << *it << endl;
                it = adjacent_find(++it, tour.end());
            }
    }
    }

    if(it!=tour.end()){
        cout << "DEU MERDA"<<endl;

        for(int i : tour){
            cout << i << " ";
        }
        exit(1);
    }
    return true;
}


vector<int> tourGen(){
    vector<int> tour;
    int depotId=Configs::customerMap.getDepotId();

        for(auto c : Configs::customerMap.getMap()){
            if(c.getId() != depotId){
                tour.push_back(c.getId());
            }
        }

        std::random_shuffle(tour.begin(), tour.end());

        unsigned tamGambi = tour.size() + Configs::customerMap.getNumberTrucks();
        int i=0, backsGap = (tour.size()+1)/Configs::customerMap.getNumberTrucks();
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
    double Rate= (double)Configs::InitialPopMutRate/(double)100;
    unsigned numIndivs=(pop.getPop().size()*Rate);
    cout <<"Rate::"<<Rate<<endl;
    cout<<"Num Indiv Mutate: "<<  numIndivs<<endl;
    for(unsigned i=0;i<Configs::InitialPopmutIterations;i++){
        for(unsigned i=0;i<numIndivs;i++){
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
        }
        for(unsigned i=0;i<numIndivs;i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation((pop.getPop()[i].getRoute())));
        }
        
    }
    pop.sortPop();
}
void applyMutation(Population &pop){
    double Rate=(double)Configs::mutationRate/(double)100;
    
    cout <<"Rate::"<<Rate<<endl;
    unsigned numIndiv=pop.getPop().size()*Rate;
    cout<<"Num Indiv Mutate: "<<numIndiv<<endl;
    if(Configs::applyWorst){
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
    int nToKeep =(int) (double)(pop.getPop().size()) * (double)(Configs::nBestToKeep/(double)100);
    for(unsigned i=(nToKeep+1);i<pop.getPop().size();i++){
        for(unsigned j=0;j<Configs::resetMutIterations;j++){
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
    validateTour(red.getRoute());
    validateTour(blue.getRoute());

    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    
    return(offspring);
}