#include "Crossover.hpp"
#include "HamiltonianCycle.hpp"
#include "GPX2.hpp"
#include "Extra.hpp"

void Crossover::crossoverPopulation(Population& pop){
    unsigned size{(unsigned)pop.getPop().size()};
    /* Population aux;
    for(unsigned i=0; i<size; i++) {
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
    }
    aux.sortPop();
    return(aux); */
    Population fullPop;

    for(unsigned i{0}; i<size; i++) {
        Tour nowT{pop.getPop()[i]};
        for(unsigned j{0}; j<size; j++) {
            if(i != j) {
                Tour offspring{Crossover::crossover(nowT, pop.getPop()[j])};
                nowT = offspring;
            }
        }
        fullPop.addNewTour(nowT);
    }
    fullPop.sortPop();
    pop = fullPop;
}

Tour Crossover::crossover(Tour& red, Tour& blue){
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    return(offspring);
}
