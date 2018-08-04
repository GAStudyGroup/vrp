#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP
#include "Tour.hpp"
#include "Population.hpp"
namespace Crossover{
    Tour crossoverGPX(Tour&, Tour&);
    void crossoverGPX_OLD(Population&);
    void crossoverGPX_5Best(Population&);

    void crossoverOX(Tour&, Tour&);
    void crossoverOX(Population&);

    
}

namespace OXCrossover{
    int findElement(vector<int>& vector, const int element);
    void swap(vector<int>& vector, const int a, const int b);
    vector<vector<int>> returnEletism(Population& pop);
}
#endif