#ifndef GACONTROL_HPP
#define GACONTROL_HPP

#include <vector>
using std::vector;
#include <fstream>
#include <ostream>

#include "Population.hpp"

//GenerationCtrl
namespace GenerationCtrl{
    Population newGeneration(Population& pop);
}

/* Control Run */
namespace RunControl {
    void initAlg(Population&);
    std::ofstream initLogFile();
    void printHeader(std::ostream&);
    void printExecutionTime(std::ostream&, double);
    bool stopAlg(Population&);
}

#endif