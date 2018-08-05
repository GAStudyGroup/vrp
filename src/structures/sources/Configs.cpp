#include "Configs.hpp"


/* Namespace Configs */
std::string Configs::file{""};
std::string Configs::pathToFile{""};
unsigned Configs::truckNumber{0};
unsigned Configs::popSize{0};
unsigned Configs::runId{0};
unsigned Configs::crossoverType{1};
int Configs::fitnessMode{4};
unsigned Configs::maxIterations{0};
unsigned Configs::optimalValue{0};
unsigned Configs::logMethod{0};
unsigned Configs::elitismNumber{5};
unsigned Configs::initialPopMethod{0};
bool Configs::withMutation{true};

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
int ResetConfigs::nBestToKeep{30};
unsigned ResetConfigs::resetMutIterations{30};

//Fitness
double Fitness::initialBest{0};

//Kmeans
int KmeansCfg::KmeansIterations{3000};