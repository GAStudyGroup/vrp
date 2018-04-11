#include "Crossover.hpp"
#include "HamiltonianCycle.hpp"
#include "GPX2.hpp"

Population Crossover::crossoverPopulation(Population& pop){
    unsigned size{(unsigned)pop.getPop().size()};
    Population aux;

    for(unsigned i=0; i<size; i++){
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
    }
    aux.sortPop();
    return(aux);
}

Tour Crossover::crossover(Tour& red, Tour& blue){
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    return(offspring);
}
