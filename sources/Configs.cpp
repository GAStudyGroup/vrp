#include "Configs.hpp"

unsigned Configs::truckNumber{0};
unsigned Configs::popSize{0};
unsigned Configs::mutIterations{0};
unsigned Configs::disturbFactor{0};
unsigned Configs::elitismNumber{0};
unsigned Configs::currentIteration{0};
unsigned Configs::maxIterations{100};

CustomerMap Configs::customerMap{CustomerMap()};
int Configs::fitnessMode{1};
double Configs::initialBest{0};