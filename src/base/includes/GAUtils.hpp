#ifndef GAUTILS_HPP
#define GAUTILS_HPP

#include <vector>
using std::vector;
#include <fstream>
#include <ostream>

#include "Population.hpp"


Population newGeneration(Population& pop);

void applyMutation(Population &pop);

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