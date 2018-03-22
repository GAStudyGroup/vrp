#ifndef FITNESS_HPP
#define FITNESS_HPP

#include <vector>

using std::vector;

double distFitness(vector<int>);

double ourFitness(vector<int>);

double advancedFitness(vector<int>);
int calcMnv();
double calcAlpha();
double calcPenalty(vector<int>);

#endif