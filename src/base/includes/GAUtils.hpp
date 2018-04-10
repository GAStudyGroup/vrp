#ifndef GAUTILS_HPP
#define GAUTILS_HPP

#include <vector>
#include <algorithm>

#include "Population.hpp"
using std::vector;

vector<int> tourGen();

Population popGen(int);

Population newGeneration(Population& pop);

void initialPopApplyMutation(Population &pop);

void applyMutation(Population &pop);

Population crossoverPopulation(Population&);

Tour crossover(Tour&, Tour&);

void popReset(Population &);

#endif