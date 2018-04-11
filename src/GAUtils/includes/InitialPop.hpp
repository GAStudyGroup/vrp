#ifndef INITIALPOP_HPP
#define INITIALPOP_HPP
#include <vector>
#include "Population.hpp"

namespace InitialPop{
    std::vector<int> tourGen();
    Population popGen(int);
    void initialPopApplyMutation(Population &pop);
}
#endif