#ifndef INITIALPOP_HPP
#define INITIALPOP_HPP
#include <vector>
#include "Population.hpp"

namespace InitialPop{
    std::vector<int> tourGen();
    Population popGen(int);
    void initialPopApplyMutation(Population &pop);
    void shuffleRoutes(Tour& tour);

    Population InitialPopRandom(int size);
    
    Population InitialPopByMutation(int size);
    
    Population InitialPopByKmeans(int size);

    Population InitialPopAdvanced(int size);
}
#endif