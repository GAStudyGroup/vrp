#include <algorithm>

#include "CapacitedKmeans.hpp"
#include "Configs.hpp"
#include "Extra.hpp"
#include "InitialPop.hpp"
#include "TourRepairer.hpp"

Population InitialPop::InitialPopByMutation(int size)
{
    Population pop;
    pop = popGen(size);
    initialPopApplyMutation(pop);
    return pop;
}

void InitialPop::initialPopApplyMutation(Population& pop)
{
    double Rate = (double)MutationCtrl::InitialPopMutRate / (double)100;
    unsigned numIndivs = (pop.getPop().size() * Rate);
    //std::cout <<"Rate:"<<Rate<<std::endl;
    //std::cout <<"Num Indiv Mutate: "<<  numIndivs<<std::endl;
    for (unsigned i = 0; i < MutationCtrl::InitialPopmutIterations; i++) {
        for (unsigned i = 0; i < numIndivs; i++) {
            Extra::applyRepair(pop.getPop()[i]);
        }
        for (unsigned i = 0; i < numIndivs; i++) {
            Extra::applyMutation(pop.getPop()[i]);
        }
        for (unsigned i = 0; i < numIndivs; i++) {
            Extra::applyRepairV4(pop.getPop()[i]);
        }
    }
}

Population InitialPop::popGen(int popSize)
{
    Population pop;
    for (int x = 0; x < popSize; x++) {
        Tour tour = Tour(tourGen());
        Extra::applyRepair(tour);
        Extra::applyOptInSubs(tour);
        pop.addNewTour(tour);
    }
    pop.sortPop();
    return (pop);
}

vector<int> InitialPop::tourGen()
{
    // vector<int> tour;
    // int depotId=Globals::customerMap.getDepotId();

    // for(auto c : Globals::customerMap.getMap()){
    //     if(c.getId() != depotId){
    //         tour.push_back(c.getId());
    //     }
    // }
    // std::shuffle(tour.begin(), tour.end(), Globals::urng);

    // unsigned tamGambi = tour.size() + Configs::truckNumber;
    // int i=0, backsGap = (tour.size()+1)/Configs::truckNumber;
    // while(tour.size() != tamGambi){
    //     tour.insert(tour.begin()+i, depotId);
    //     i+=backsGap+1;
    // }
    // return tour;
    int depotId{ Globals::customerMap.getDepotId() };
    vector<int> tour;

    for (Customer c : Globals::customerMap.getMap()) {
        if (c.getId() != depotId) {
            tour.push_back(c.getId());
        }
    }
    //std::shuffle(tour.begin(), tour.end(), Globals::urng);
    for (unsigned i{ 0 }; i < Configs::truckNumber; i++) {
        tour.push_back(depotId);
    }
    std::shuffle(tour.begin(), tour.end(), Globals::urng);
    return (tour);
}

void InitialPop::shuffleRoutes(Tour& tour)
{
    vector<vector<int>> routes = tour.explodeSubTours();
    int empty = Configs::truckNumber - routes.size();
    int depotId = Globals::customerMap.getDepotId();
    for (auto& route : routes) {
        std::shuffle(route.begin(), route.end(), Globals::urng);
    }

    vector<int> emptyAux;
    for (int i = 0; i < empty; i++) {
        routes.push_back(emptyAux);
    }
    for (auto& route : routes) {
        route.emplace(route.begin(), depotId);
    }
    tour = TourRepairer::tourRebuilder(routes);
}

Population InitialPop::InitialPopByKmeans(int size)
{
    vector<Tour> population;
    Tour baseTour = tourGen();
    for (int i = 0; i < 50; i++) {
        Extra::applyRepair(baseTour);
    }
    baseTour = CapacitedKmeans::run(baseTour);
    for (int i = 0; i < size; i++) {
        shuffleRoutes(baseTour);
        population.push_back(baseTour);
    }
    return population;
}

Population InitialPop::InitialPopAdvanced(int size)
{
    Population pop = InitialPopByKmeans(size);
    for (unsigned i = 0; i < 15; i++) {
        Extra::applyMutation(pop);
    }
    return pop;
}

Population InitialPop::InitialPopRandom(int size)
{
    vector<Tour> tours;
    for (int i = 0; i < size; i++) {
        tours.push_back(tourGen());
    }
    return tours;
}