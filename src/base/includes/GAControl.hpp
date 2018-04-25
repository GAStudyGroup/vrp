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
    Population basicGenerationGPX(Population &pop);
    Population basicGenerationOX(Population &pop);

    Population generation(Population &pop);
}

/* Control Run */
namespace RunControl {
    void initAlg(Population&);
    std::ofstream initLogFile();
    void printHeader(std::ostream&);
    void printExecutionTime(std::ostream&, double);
    void printFooter(std::ostream&, Tour&);
    bool stopAlg(Population&);
}

#endif