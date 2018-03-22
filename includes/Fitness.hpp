#ifndef FITNESS_HPP
#define FITNESS_HPP

#include <vector>

using std::vector;

double distFitness();

double ourFitness();

double advancedFitness(vector<int>);
int calcMnv(vector<int>);
double calcPenalty(vector<int>);

#endif