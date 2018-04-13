#include "Extra.hpp"
#include "Configs.hpp"
#include "TourRepairer.hpp"
#include "InitialPop.hpp"
#include "Mutation.hpp"
#include "Opt.hpp"


void Extra::popReset(Population &pop){ // OLD
    unsigned size{(unsigned) pop.getPop().size()};
    unsigned nToKeep{(unsigned)std::round(((size)*ResetConfigs::nBestToKeep)/100)};
    std::cout << "NToKeep " << nToKeep << std::endl;  

    Population aux;
    aux.getPop().insert(aux.getPop().end(), pop.getPop().begin(), pop.getPop().begin()+nToKeep);

    for(unsigned i{nToKeep}; i<size; i++) {
        Tour t{InitialPop::tourGen()};
        std::cout << "==================== Gerado:======================== \n" << t << std::endl;
        applyRepair(t);
        std::cout << "========================Reparado========================\n" << t << std::endl;
        applyOptInSubs(t);
        std::cout << "========================OPT========================\n" << t << std::endl;
        aux.addNewTour(t);
    }
    pop = aux;
    pop.sortPop();
}

/* void Extra::popReset(Population &pop){ // NEW
    unsigned size{(unsigned) pop.getPop().size()};
    unsigned nToKeep{(unsigned)std::round(((size)*ResetConfigs::nBestToKeep)/100)};

    Population aux;
    aux.getPop().insert(aux.getPop().end(), pop.getPop().begin(), pop.getPop().begin()+nToKeep);

    for(unsigned i{nToKeep}; i<size; i++) {
        Tour t{InitialPop::tourGen()};
        //std::cout << "==================== Gerado:======================== \n" << t << std::endl;
        int countRepair{0};
        while(!t.isValid() && countRepair!=5) {
            //std::cout << "REPARA A MERDA"<<std::endl;
            applyRepair(t);
            countRepair++;
        }
        //std::cout << "========================Reparado========================\n" << t << std::endl;
        applyOptInSubs(t);
        //std::cout << "========================OPT========================\n" << t << std::endl;
        aux.addNewTour(t);
    }
    pop = aux;
    pop.sortPop();
} */

void Extra::applyMutation(Population &pop){
    double Rate=(double)MutationCtrl::mutationRate/(double)100;
    //std::cout<<"Rate::"<<Rate<<std::endl;
    unsigned numIndiv=pop.getPop().size()*Rate;
    //std::cout<<"Num Indiv Mutate: "<<numIndiv<<std::endl;
    if(MutationCtrl::applyWorst){
        for(unsigned i=pop.getPop().size()-1;i>(pop.getPop().size()-(numIndiv));i--){
            applyMutation(pop.getPop()[i]);
        }
    }else{
        for(unsigned i=0;i<numIndiv;i++){
            applyMutation(pop.getPop()[i]);
        }  
    }
    pop.sortPop();
}

void Extra::applyMutation(Tour& tour){
    tour=Tour(Mutation::evaluateMutation(tour.getRoute()));
    applyRepair(tour);
}

void Extra::applyRepair(Population& pop){
    for(unsigned i=0;i<pop.getPop().size();i++){
        applyRepair(pop.getPop()[i]);
    }
    pop.sortPop();
}
void Extra::applyRepair(Tour& tour){
    tour=TourRepairer::repairTour(tour);
}

void Extra::applyOptInSubs(Tour& t) {
    int depotId{Globals::customerMap.getDepotId()};
    int empty{t.getEmptySubtoursNumber()};
    vector<vector<int>> subTours{t.explodeSubTours()};
    vector<int> newT;
    newT.reserve(t.getRoute().size());

    for(vector<int> &subT : subTours) {
        subT = Opt::optimize(subT);
    }

    for(vector<int> subT : subTours) {
        newT.push_back(depotId);
        newT.insert(newT.end(), subT.begin(), subT.end());
    }

    while(empty > 0) {
        newT.push_back(depotId);
        empty--;
    }

    t = Tour(newT);
}
