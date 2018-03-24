#include "Configs.hpp"

unsigned Configs::truckNumber{0};
unsigned Configs::popSize{0};
unsigned Configs::disturbFactor{0};
unsigned Configs::elitismNumber{0};
unsigned Configs::currentIteration{0};
unsigned Configs::maxIterations{2000};

CustomerMap Configs::customerMap{CustomerMap()};
int Configs::fitnessMode{1};
double Configs::initialBest{0};

unsigned Configs::InitialPopmutIterations{10};
unsigned Configs::InitialPopMutRate{100};

unsigned Configs::mutationRate{50};
bool Configs::applyWorst{false};