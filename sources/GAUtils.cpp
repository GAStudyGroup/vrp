#include "GAUtils.hpp"
#include "Configs.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"
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
    double Rate= Configs::InitialPopMutRate/100;
    for(unsigned i=0;i<Configs::InitialPopmutIterations;i++){
        for(unsigned i=0;i<(pop.getPop().size()*Rate);i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation((pop.getPop()[i].getRoute())));
        }
    }
}
void applyMutation(Population &pop){
    double Rate=Configs::mutationRate/100;
    if(Configs::applyWorst){
        for(unsigned i=pop.getPop().size();i>pop.getPop().size()*Rate;i--){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation(pop.getPop()[i].getRoute()));
        }
    }else{
        for(unsigned i=0;i<pop.getPop().size()*Rate;i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation(pop.getPop()[i].getRoute()));
        }  
    }
}

Population newGeneration(Population& pop){
    Population newPop;

    for(unsigned i=0; i<pop.getPop().size(); i++){
        Tour offs;
        cout <<"Tour1 Entrando: "<<pop.getPop()[i]<<endl;
        cout <<"Tour2 Entrando: "<<pop.getPop()[(i+1)%pop.getPop().size()]<<endl;
        offs = GPX2::crossover(pop.getPop()[i], pop.getPop()[(i+1)%pop.getPop().size()]);
        newPop.addNewTour(offs);
    }
    newPop.sortPop();
    applyMutation(newPop);
    newPop.sortPop();
    return(newPop);
}