#ifndef FITNESS_HPP
#define FITNESS_HPP

#include <vector>

using std::vector;

double distFitness(vector<int>&);

double ourFitness(vector<int>&);
// return a fitness value of a route inside the tour
double subFitness(vector<int>&, double&);
// return a fitness value, with penalty, of a route inside the tour
double subFitnessPenalty(vector<int>&, double&);


double advancedFitness(vector<int>&);
int calcMnv();
double calcAlpha();
double calcPenalty(vector<int>&);

#endif