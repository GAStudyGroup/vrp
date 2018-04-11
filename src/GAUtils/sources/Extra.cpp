#include "Extra.hpp"
#include "Configs.hpp"
#include "TourRepairer.hpp"
#include "InitialPop.hpp"
#include "Mutation.hpp"

void Extra::popReset(Population &pop){
    int nToKeep =(int) (double)(pop.getPop().size()) * (double)(ResetConfigs::nBestToKeep/(double)100);
    for(unsigned i=(nToKeep+1);i<pop.getPop().size();i++){
        for(unsigned j=0;j<ResetConfigs::resetMutIterations;j++){
            pop.getPop()[i]=InitialPop::tourGen();
            applyRepair(pop.getPop()[i]);
            applyMutation(pop.getPop()[i]);
            applyRepair(pop.getPop()[i]);
        }
    }
    pop.sortPop();
}

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
    tour=Tour(Mutation().evaluateMutation(tour.getRoute()));
    applyRepair(tour);
}

void Extra::applyRepair(Population& pop){
    for(unsigned i=0;i<pop.getPop().size();i++){
        applyRepair(pop.getPop()[i]);
    }
    pop.sortPop();
}
void Extra::applyRepair(Tour& tour){
    tour=TourRepairer().repairTour(tour);
}
