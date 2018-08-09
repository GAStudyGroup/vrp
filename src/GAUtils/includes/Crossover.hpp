#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP
#include "Tour.hpp"
#include "Population.hpp"

#include <vector>
using std::vector;

#include <set>
using std::set;
namespace Crossover{
    Tour crossoverGPX(Tour&, Tour&);
    void crossoverGPX_OLD(Population&);
    void crossoverGPX_5Best(Population&);
    void crossoverOX_Elitism(Population&);
    void crossoverRBX(Population&);    
}

namespace OX{
    void crossover(Tour  t1, Tour t2);
}

namespace RBX{
    Tour crossover(Tour t1, Tour t2);
    vector<vector<int>> getValidRoutes(Tour t);   
    vector<vector<int>> mergeValidRoutes(vector<vector<int>> valid1 ,vector<vector<int>> valid2);
    Tour rebuild(vector<vector<int>>);

    set<int> getDuplicated(vector<vector<int>>);
    set<int> getAssigned(vector<vector<int>>);
    set<int> getUnassgined(vector<vector<int>>);
    void removeDuplicated(vector<vector<int>>& routes, set<int>& duplicated);
    
    void remerge(vector<vector<int>>& routes);
    vector<int> mergeRoutes(vector<int> route1, vector<int> route2);
    void insertUnassigned(vector<vector<int>>& routes, set<int>& unassigned);
}

#endif