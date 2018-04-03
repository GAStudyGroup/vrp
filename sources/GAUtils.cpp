#include "GAUtils.hpp"
#include "Configs.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"
#include "HamiltonianCycle.hpp"

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
    double Rate= Configs::InitialPopMutRate/100;
    for(unsigned i=0;i<Configs::InitialPopmutIterations;i++){
        for(unsigned i=0;i<(pop.getPop().size()*Rate);i++){
            pop.getPop()[i]=Tour(Mutation().evaluateMutation((pop.getPop()[i].getRoute())));
        }
    }
    pop.sortPop();
}
void applyMutation(Population &pop){
    double Rate=Configs::mutationRate/100;
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

/* Population newGeneration(Population& pop){
    Population newPop;
    int defaultSize= (Configs::customerMap.getMap().size() + Configs::truckNumber)-1;
    for(unsigned i=0; i<pop.getPop().size(); i++){
        Tour offs;
        cout <<"Tour1 Entrando: "<<pop.getPop()[i]<<endl;
        cout <<"Tour2 Entrando: "<<pop.getPop()[(i+1)%pop.getPop().size()]<<endl;
        // offs = GPX2::crossover(pop.getPop()[i], pop.getPop()[(i+1)%pop.getPop().size()]);
        // if(offs.getRoute().size()>defaultSize){
        //     cout <<"merda"<<endl;
        //     cout <<"Tour saída: " <<offs<<endl;
        //     exit(-1);
        // }
        // newPop.addNewTour(offs);
    }
    newPop.sortPop();
    applyMutation(newPop);
    newPop.sortPop();
    return(newPop);
} */

Population newGeneration(Population& pop){
    int defaultSize{Configs::customerMap.getMap().size() + Configs::truckNumber -1};
    
    Population newPop{crossoverPopulation(pop)};

    newPop.sortPop();
    applyMutation(newPop);
    newPop.sortPop();
    return(newPop);
}

Population crossoverPopulation(Population& pop){
    unsigned size{pop.getPop().size()};
    Population aux;

    for(unsigned i=0; i<size; i++){
        cout << "CROSS"<<endl;
        cout << "RED\n"<<pop.getPop()[i]<<endl;
        cout << "BLUE\n"<<pop.getPop()[(i+1)%size]<<endl;
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
        cout << "FILHO\n"<<auxT<<endl;
    }
    return(aux);
}

Tour crossover(Tour& red, Tour& blue){
    validateTour(red.getRoute());
    validateTour(blue.getRoute());

    cout << "CHAMADA CROSS"<<endl;
    cout << "RED\n"<<red<<endl;
    cout << "BLUE\n"<<blue<<endl;

    cout<<"-------------------Entrando hamiltonian------------------"<<endl;
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    if(!Mutation().validateTour(Tour(parents.first).getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId())){
        cout <<"merda depois da hamiltonian";
        exit(-1);
    }
    if(!Mutation().validateTour(Tour(parents.second).getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId())){
        cout <<"merda depois da hamiltonian";
        exit(-1);
    }
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    
    if(!Mutation().validateTour(offspring.getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId())){
        cout <<"merda depois do gpx";
        exit(-1);
    }
    
    return(offspring);
}