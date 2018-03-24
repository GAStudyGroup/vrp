#ifndef GAUTILS_HPP
#define GAUTILS_HPP

#include <vector>
#include <ctime>
#include <algorithm>

#include "Population.hpp"
using std::vector;
using std::random_shuffle;

vector<int> tourGen();
Population popGen(int);
Population newGeneration(Population& pop);
void initialPopApplyMutation(Population &pop);
void applyMutation(Population &pop);
#endif