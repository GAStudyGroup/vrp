#include "Configs.hpp"


/* Namespace Configs */
std::string Configs::file{""};
std::string Configs::pathToFile{""};
unsigned Configs::truckNumber{0};
unsigned Configs::popSize{0};
unsigned Configs::runId{0};
unsigned Configs::crossoverType{0};
int Configs::fitnessMode{2};
unsigned Configs::maxIterations{0};
unsigned Configs::optimalValue{0};
unsigned Configs::logMethod{0};

/* Namespace Globals */
CustomerMap Globals::customerMap{CustomerMap()};
std::mt19937 Globals::urng;
unsigned Globals::currentIteration{0};
std::ofstream* Globals::debugLogFile;

// Mutation Configs
unsigned MutationCtrl::InitialPopmutIterations{6};
unsigned MutationCtrl::InitialPopMutRate{100};
unsigned MutationCtrl::mutationRate{50};
bool MutationCtrl::applyWorst{false};

//Mutation aux
vector<Tour> MutationCtrl::mutationMaxedList;

//Reset
int ResetConfigs::nBestToKeep{10};
unsigned ResetConfigs::resetMutIterations{30};

//Fitness
double Fitness::initialBest{0};