#include "Configs.hpp"

std::string Configs::file{""};
std::string Configs::pathToFile{""};
unsigned Configs::truckNumber{0};
unsigned Configs::popSize{0};
unsigned Configs::runId{0};
unsigned Configs::crossoverType{0};
unsigned Configs::currentIteration{0};
unsigned Configs::maxIterations{0};
 
CustomerMap Configs::customerMap{CustomerMap()};
int Configs::fitnessMode{1};
double Configs::initialBest{0};

unsigned Configs::InitialPopmutIterations{10};
unsigned Configs::InitialPopMutRate{100};

unsigned Configs::mutationRate{50};
bool Configs::applyWorst{false};

vector<Tour> Configs::mutationMaxedList;
unsigned Configs::resetMutIterations{1};
int Configs::nBestToKeep{10};

std::mt19937 Configs::urng;