#include "Crossover.hpp"
#include "Configs.hpp"
#include "Extra.hpp"
#include "GPX2.hpp"
#include "HamiltonianCycle.hpp"
#include "TourUtils.hpp"

#include <algorithm>

#include <random>
using std::uniform_int_distribution;

void Crossover::crossoverGPX_5Best(Population &pop)
{
    Population newPop;
    pop.sortPop();
    vector<Tour> bestFive{pop.getPop().begin(), pop.getPop().begin() + 5};

    for (Tour t : bestFive)
    {
        newPop.addNewTour(t);
    }

    for (unsigned i{0}; i < 5; i++)
    {
        Tour nowT{bestFive[i]};
        for (unsigned j{0}; j < pop.getPop().size(); j++)
        {
            if (i != j)
            {
                Tour offspring{Crossover::crossoverGPX(nowT, pop.getPop()[j])};
                if (offspring.isValid())
                {
                    nowT = offspring;
                }
            }
        }
        newPop.addNewTour(nowT);
    }
    pop = newPop;
}

void Crossover::crossoverGPX_OLD(Population &pop)
{
    unsigned size{(unsigned)pop.getPop().size()};
    /* Population aux;
    for(unsigned i=0; i<size; i++) {
        Tour auxT{crossover(pop.getPop()[i], pop.getPop()[(i+1)%size])};
        aux.addNewTour(auxT);
    }
    aux.sortPop();
    return(aux); */
    Population fullPop;

    int test{0};
    for (unsigned i{0}; i < size; i++)
    {
        Tour nowT{pop.getPop()[i]};
        for (unsigned j{0}; j < size; j++)
        {
            if (i != j)
            {
                Tour offspring{Crossover::crossoverGPX(nowT, pop.getPop()[j])};
                nowT = offspring;
                test++;
            }
        }
        fullPop.addNewTour(nowT);
    }
    fullPop.sortPop();
    pop = fullPop;
}

Tour Crossover::crossoverGPX(Tour &red, Tour &blue)
{
    HamiltonianCycle::parentsHamiltonian parents{HamiltonianCycle::toHamiltonianCycle(red, blue)};
    Tour offspring{GPX2::crossover(parents.first, parents.second)};
    return (offspring);
}

void Crossover::crossoverOX_Elitism(Population &pop)
{
    Population newPop;
    pop.sortPop();
    newPop.getPop().insert(newPop.getPop().end(), pop.getPop().begin(), pop.getPop().begin() + 10);
    std::shuffle(pop.getPop().begin(), pop.getPop().end(), Globals::urng);

    for (unsigned i{0}; i < pop.getPop().size(); i++)
    {
        if (i == pop.getPop().size() - 1)
        {
            OX::crossover(pop.getPop()[i], pop.getPop()[0]);
        }
        else
        {
            OX::crossover(pop.getPop()[i], pop.getPop()[i + 1]);
        }
    }
    pop.sortPop();
    newPop.getPop().insert(newPop.getPop().end(), pop.getPop().begin(), pop.getPop().begin() + (Configs::popSize - 10));
    pop = newPop;
    pop.sortPop();
}

//OX functions
void OX::crossover(Tour t1, Tour t2)
{
    uniform_int_distribution<int> dist(0, t1.getRoute().size() - 1);
    vector<int> tmp1, tmp2;
    unsigned cut1, cut2;

    do
    {
        cut1 = (unsigned)dist(Globals::urng);
        cut2 = (unsigned)dist(Globals::urng);
        if (cut2 < cut1)
        {
            unsigned tmp{cut2};
            cut2 = cut1;
            cut1 = tmp;
        }
        // cout<<"cut1 "<<cut1<<" cut2 "<<cut2<<endl;
    } while (cut1 == cut2);

    for (unsigned i{cut1}; i <= cut2; i++)
    {
        tmp2.push_back(t1.getRoute()[i]);
        tmp1.push_back(t2.getRoute()[i]);
    }

    for (unsigned i = 0; i < tmp1.size(); i++)
    {
        t1.getRoute()[find(t1.getRoute().begin(), t1.getRoute().end(), tmp1[i]) - t1.getRoute().begin()] = -1;
        t2.getRoute()[find(t2.getRoute().begin(), t2.getRoute().end(), tmp2[i]) - t2.getRoute().begin()] = -1;
    }

    for (unsigned i = 0; i < t1.getRoute().size(); i++)
    {
        if (t1.getRoute()[i] != -1)
        {
            tmp1.push_back(t1.getRoute()[i]);
        }
        if (t2.getRoute()[i] != -1)
        {
            tmp2.push_back(t2.getRoute()[i]);
        }
    }

    t1 = tmp1;
    t2 = tmp2;
};
//--------End OX Functions---------

//--------Start of RBX Crossover----
Tour RBX::crossover(Tour t1, Tour t2)
{
    set<int> assigned;
    set<int> duplicated;
    set<int> unassigned;
    // Identificar todas as rotas válidas
    vector<vector<int>> validRoutes1 = getValidRoutes(t1);
    vector<vector<int>> validRoutes2 = getValidRoutes(t2);

    // Realizar o merge de rotas válidas
    auto merged = mergeValidRoutes(validRoutes1, validRoutes2);

    //Identificar duplicados
    duplicated = getDuplicated(merged);
    // Identificar quais já foram inseridos
    assigned = getAssigned(merged);
    // Remover os duplicados
    removeDuplicated(merged, duplicated);
    /* Necessário agrupar rotas pequenas para que o número de rotas fique menor ou 
    igual ao numero de trucks */
    
    // Identificar quais estão faltando 
    unassigned = getUnassgined(merged);
    vector<int> batata(unassigned.begin(),unassigned.end());
    std::cout <<"Nao Inseridos"<< std::endl;
    TourUtils::printRoute(batata);
    // Inserir os que estão faltando

    // Reconstruir o tour
    Tour offSpring = rebuild(merged);
    std::cout << offSpring << std::endl;
    return offSpring;
}
vector<vector<int>> RBX::getValidRoutes(Tour tour)
{
    vector<vector<int>> validRoutes;

    for (auto route : tour.explodeSubTours())
    {
        if (TourUtils::isRouteValid(route))
        {
            validRoutes.push_back(route);
        }
    }
    return validRoutes;
}
vector<vector<int>> RBX::mergeValidRoutes(vector<vector<int>> valid1, vector<vector<int>> valid2)
{
    vector<vector<int>> merged;

    for (auto route : valid1)
    {
        merged.push_back(route);
    }
    for (auto route : valid2)
    {
        merged.push_back(route);
    }

    return merged;
}

Tour RBX::rebuild(vector<vector<int>> routes)
{
    vector<int> newRoute;

    // Add a depot in the start of each route
    for (auto &route : routes)
    {
        route.emplace(route.begin(), Globals::customerMap.getDepotId());
    }
    //Restore empty routes
    //Finish the remerge fucntion before uncomment this code
    // auto empty = Configs::truckNumber - routes.size();
    // if(empty>0){
    //     vector<int> aux;
    //     aux.push_back(Globals::customerMap.getDepotId());
    //     for(int i = 0; i < empty; i++){
    //         routes.push_back(aux);
    //     }
    // }
    //Insert them all in a single vector
    for (auto route : routes)
    {
        for (auto el : route)
        {
            newRoute.push_back(el);
        }
    }
    // Transform in a tour and return
    return newRoute;
}

set<int> RBX::getAssigned(vector<vector<int>> routes)
{
    set<int> assigned;
    for (auto route : routes)
    {
        for (auto el : route)
        {
            assigned.insert(el);
        }
    }
    return assigned;
}

set<int> RBX::getDuplicated(vector<vector<int>> routes)
{
    set<int> duplicated;
    vector<int> route = rebuild(routes).getRoute();
    int minEl = *std::min_element(route.begin(), route.end(), [](int a, int b) {
        return b > a;
    });
    int maxEl = *std::max_element(route.begin(), route.end(), [](int a, int b) {
        return b > a;
    });
    for (auto i = minEl; i <= maxEl; i++)
    {
        if (i != Globals::customerMap.getDepotId())
        {
            if (std::count(route.begin(), route.end(), i) > 1)
            {
                duplicated.insert(i);
            }
        }
    }

    return duplicated;
}

// Removes the first duplicated found, the heuristic can be improved in the future
void RBX::removeDuplicated(vector<vector<int>> &routes, set<int> &duplicated)
{
    auto routesClone = routes;
    for (unsigned i = 0; i < routesClone.size(); i++)
    {
        for (auto el : routesClone[i])
        {
            auto dupPos = duplicated.find(el);
            if (dupPos != duplicated.end())
            {
                duplicated.erase(dupPos);
                routes[i].erase(std::find(routes[i].begin(), routes[i].end(), el));
            }
        }
    }
}

// Get the unassigned customers
set<int> RBX::getUnassgined(vector<vector<int>> routes){
    auto assigned = getAssigned(routes);
    set<int> unassigned;
    for(auto customer : Globals::customerMap.getMap()){
        if(customer.getId() != Globals::customerMap.getDepotId()){
            if(assigned.find(customer.getId())==assigned.end()){
                unassigned.insert(customer.getId());
            }
        }
    }
    return unassigned;
}

//Remerge routes until the number of routes be less  or equal the number of trucks
void RBX::remerge(vector<vector<int>>){
    // to be implemented
}
//--------End of RBX Crossover-----