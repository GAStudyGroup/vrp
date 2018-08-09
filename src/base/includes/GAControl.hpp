#ifndef GACONTROL_HPP
#define GACONTROL_HPP

#include <vector>
using std::vector;
#include <fstream>
#include <ostream>

#include "Population.hpp"

//GenerationCtrl
namespace GenerationCtrl{
    Population generationRBX(Population &pop);
    Population generationForFinalTests(Population &pop);
    Population newGeneration(Population& pop);
    Population basicGenerationGPX(Population &pop);
    Population basicGenerationOX(Population &pop);
    Population GenerationOXGPX(Population &pop);
    Population generation(Population &pop);
}

namespace FinalTests {
    Population generationOX_WithoutMutation(Population&);
    Population generationGPX_WithoutMutation(Population&);

    Population generationOX_WithMutation(Population&);
    Population generationGPX_WithMutation(Population&);
}

/* Control Run */
namespace RunControl {
    Population generatePopulation(unsigned);
    void initAlg(Population&);
    std::ofstream initLogFile();
    void printHeader(std::ostream&);
    void printExecutionTime(std::ostream&, double);
    void printFooter(std::ostream&, Tour&);
    bool stopAlg(Population&);
}

#endif