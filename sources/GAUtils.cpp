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
    cout <<"Rate::"<<Rate<<endl;
    for(unsigned i=0;i<Configs::InitialPopmutIterations;i++){
        for(unsigned i=0;i<(pop.getPop().size()*Rate);i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation((pop.getPop()[i].getRoute())));
        }
        for(unsigned i=0;i<(pop.getPop().size()*Rate);i++){
            pop.getPop()[i]=TourRepairer().repairTour(pop.getPop()[i]);
        }
    }
    pop.sortPop();
}
void applyMutation(Population &pop){
    double Rate=(double)Configs::mutationRate/(double)100;
    cout <<"Rate::"<<Rate<<endl;
    if(Configs::applyWorst){
        for(unsigned i=pop.getPop().size()-1;i>(pop.getPop().size()-(pop.getPop().size()*Rate));i--){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation(pop.getPop()[i].getRoute()));
        }
    }else{
        for(unsigned i=0;i<pop.getPop().size()*Rate;i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation(pop.getPop()[i].getRoute()));
        }  
    }
}
void popReset(Population &pop){

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
    unsigned size=pop.getPop().size();
    Population aux;

    for(unsigned i=0; i<size; i++){
        // cout << "CROSS"<<endl;
        // cout << "RED\n"<<pop.getPop()[i]<<endl;
        // cout << "BLUE\n"<<pop.getPop()[(i+1)%size]<<endl;
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
        // cout << "FILHO\n"<<auxT<<endl;
    }
    return(aux);
}

Tour crossover(Tour& red, Tour& blue){
    validateTour(red.getRoute());
    validateTour(blue.getRoute());

    // cout << "CHAMADA CROSS"<<endl;
    //  cout<<"-------------------Entrando hamiltonian------------------"<<endl;
    //  cout << "RED\n"<<red<<endl;
    //  cout << "BLUE\n"<<blue<<endl;

    
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    //  cout<<"---------------------Saindo Hamiltonian--------------------"<<endl;
    Tour first=Tour(parents.first);
    Tour second=Tour(parents.second);
    //  cout <<"First:" << first<<endl;
    //  cout <<"Second:" <<second<<endl;

    
    // if(!Mutation().validateTour(Tour(parents.first).getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId())){
    //     cout <<"merda depois da hamiltonian\n";
    //     exit(-1);
    // }
    // if(!Mutation().validateTour(Tour(parents.second).getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId())){
    //     cout <<"merda depois da hamiltonian\n";
    //     exit(-1);
    // }
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    
    // if(!Mutation().validateTour(offspring.getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId())){
    //     cout <<"merda depois do gpx";
    //     exit(-1);
    // }
    //  cout<<"----------------Saindo GPX-----------------"<<endl;;
    //  cout <<offspring<<endl;
    return(offspring);
}