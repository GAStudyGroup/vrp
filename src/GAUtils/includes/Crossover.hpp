#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP
#include "Tour.hpp"
#include "Population.hpp"
namespace Crossover{
    Tour crossover(Tour&, Tour&);
    void crossoverPopulation(Population&);
}
#endif