#ifndef GAUTILS_HPP
#define GAUTILS_HPP

#include <vector>
using std::vector;
#include <algorithm>
#include <fstream>
#include <ostream>

#include "Population.hpp"

vector<int> tourGen();

Population popGen(int);

Population newGeneration(Population& pop);

void initialPopApplyMutation(Population &pop);

void applyMutation(Population &pop);

Population crossoverPopulation(Population&);

Tour crossover(Tour&, Tour&);

void popReset(Population &);

/* Control Run */
namespace RunControl {
    void initAlg(Population&);
    std::ofstream initLogFile();
    void printHeader(std::ostream&);
    void printExecutionTime(std::ostream&, double);
    bool stopAlg(Population&);
}

#endif