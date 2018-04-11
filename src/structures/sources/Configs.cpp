#include "Configs.hpp"


/* Namespace Configs */
std::string Configs::file{""};
std::string Configs::pathToFile{""};
unsigned Configs::truckNumber{0};
unsigned Configs::popSize{0};
unsigned Configs::runId{0};
unsigned Configs::crossoverType{0};
unsigned Configs::currentIteration{0};
unsigned Configs::maxIterations{0};
 
int Configs::fitnessMode{1};
double Configs::initialBest{0};

unsigned Configs::InitialPopmutIterations{50};
unsigned Configs::InitialPopMutRate{100};

unsigned Configs::mutationRate{50};
bool Configs::applyWorst{false};

vector<Tour> Configs::mutationMaxedList;
unsigned Configs::resetMutIterations{30};
int Configs::nBestToKeep{5};

/* Namespace Globals */
CustomerMap Globals::customerMap{CustomerMap()};
std::mt19937 Globals::urng;