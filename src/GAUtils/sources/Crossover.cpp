#include <algorithm>
#include <random>
using std::uniform_int_distribution;
#include "Crossover.hpp"
#include "Configs.hpp"
#include "Extra.hpp"
#include "GPX2.hpp"
#include "HamiltonianCycle.hpp"

void Crossover::crossoverGPX_5Best(Population& pop)
{
    Population newPop;
    pop.sortPop();
    vector<Tour> bestFive{ pop.getPop().begin(), pop.getPop().begin() + 5 };

    for (Tour t : bestFive) {
        newPop.addNewTour(t);
    }

    for (unsigned i{ 0 }; i < 5; i++) {
        Tour nowT{ bestFive[i] };
        for (unsigned j{ 0 }; j < pop.getPop().size(); j++) {
            if (i != j) {
                Tour offspring{ Crossover::crossoverGPX(nowT, pop.getPop()[j]) };
                if (offspring.isValid()) {
                    nowT = offspring;
                }
            }
        }
        newPop.addNewTour(nowT);
    }
    pop = newPop;
}

void Crossover::crossoverGPX_OLD(Population& pop)
{
    unsigned size{ (unsigned)pop.getPop().size() };
    /* Population aux;
    for(unsigned i=0; i<size; i++) {
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
    }
    aux.sortPop();
    return(aux); */
    Population fullPop;

    int test{ 0 };
    for (unsigned i{ 0 }; i < size; i++) {
        Tour nowT{ pop.getPop()[i] };
        for (unsigned j{ 0 }; j < size; j++) {
            if (i != j) {
                Tour offspring{ Crossover::crossoverGPX(nowT, pop.getPop()[j]) };
                nowT = offspring;
                test++;
            }
        }
        fullPop.addNewTour(nowT);
    }
    fullPop.sortPop();
    pop = fullPop;
}

Tour Crossover::crossoverGPX(Tour& red, Tour& blue)
{
    HamiltonianCycle::parentsHamiltonian parents{ HamiltonianCycle::toHamiltonianCycle(red, blue) };
    Tour offspring{ GPX2::crossover(parents.first, parents.second) };
    return (offspring);
}

void Crossover::crossoverOX_Elitism(Population& pop) {
    Population newPop;
    pop.sortPop();
    newPop.getPop().insert(newPop.getPop().end(), pop.getPop().begin(), pop.getPop().begin()+10);
    std::shuffle(pop.getPop().begin(), pop.getPop().end(), Globals::urng);

    for(unsigned i{0}; i<pop.getPop().size(); i++) {
        if (i == pop.getPop().size() - 1) {
            OX::crossover(pop.getPop()[i], pop.getPop()[0]);
        } else {
            OX::crossover(pop.getPop()[i], pop.getPop()[i + 1]);
        }
    }
    pop.sortPop();
    newPop.getPop().insert(newPop.getPop().end(), pop.getPop().begin(), pop.getPop().begin()+(Configs::popSize - 10));
    pop = newPop;
    pop.sortPop();
}

//OX functions
void OX::crossover(Tour t1, Tour t2){
    uniform_int_distribution<int> dist(0,t1.getRoute().size()-1);
    vector<int> tmp1,tmp2;
    unsigned cut1, cut2;
    
    do{
        cut1 = (unsigned)dist(Globals::urng);
        cut2 = (unsigned)dist(Globals::urng);
        if(cut2<cut1){
            unsigned tmp{cut2};
            cut2 = cut1;
            cut1 = tmp;
        }
        // cout<<"cut1 "<<cut1<<" cut2 "<<cut2<<endl;
    }while(cut1==cut2);


    for (unsigned i{ cut1 }; i <= cut2; i++) {
        tmp2.push_back(t1.getRoute()[i]);
        tmp1.push_back(t2.getRoute()[i]);
    }


    for(unsigned i=0;i<tmp1.size();i++){
        t1.getRoute()[find(t1.getRoute().begin(),t1.getRoute().end(),tmp1[i]) - t1.getRoute().begin()] = -1;
        t2.getRoute()[find(t2.getRoute().begin(),t2.getRoute().end(),tmp2[i]) - t2.getRoute().begin()] = -1;
    }

    for(unsigned i=0;i<t1.getRoute().size();i++){
        if(t1.getRoute()[i]!=-1){
            tmp1.push_back(t1.getRoute()[i]);
        }
        if(t2.getRoute()[i]!=-1){
            tmp2.push_back(t2.getRoute()[i]);
        }
    }

    t1 = tmp1;
    t2 = tmp2;
};
//--------End OX Functions---------

Tour RBX::crossover (Tour t1, Tour t2){
    // Identificar todas as rotas válidas
    // Realizar o merge de todas
    // Remover os duplicados
    // Inserir os que estão faltando
    
}