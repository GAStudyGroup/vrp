#ifndef CROSSOVER_HPP
#define CROSSOVER_HPP
#include "Tour.hpp"
#include "Population.hpp"
namespace Crossover{
    Tour crossoverGPX(Tour&, Tour&);
    void crossoverGPX_OLD(Population&);
    void crossoverGPX_5Best(Population&);

    void crossoverOX_Elitism(Population&);
    
}

namespace OX{
    void crossover(Tour  t1, Tour t2);
}

namespace RBX{
    Tour crossover(Tour t1, Tour t2);
}

#endif