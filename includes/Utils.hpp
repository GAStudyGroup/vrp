#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include <limits>
#include <utility>

#include "Map.hpp"
#include "Population.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;


// SECTION TO FITNESS CALCULATION
// Main function to calculate fitness, will return the value
double getTourFitness(vector<int>&, Map&);
double getTourDistance(vector<int>&, Map&);
// return the charge used of route inside the tour
double getSubCharge(vector<int>&, Map&);
// return a fitness value of a route inside the tour
double subFitness(vector<int>&, double&, Map&);
// return a fitness value, with penalty, of a route inside the tour
double subFitnessPenalty(vector<int>&, double&, Map&);
// return a distance used in route 
double getSubDistance(vector<int>&, Map&);


// return the euclidian distance beetween two points in map
double distance(const int, const int, Map&);

// Return a vector of each tour (route) in cromossome, like a "explode" function for strings
vector<vector<int>> explodeSubTours(vector<int>, int);

// Swap two values of a vector
void swap(vector<int>&, const int, const int);

// Return the position of element in vector
int findElement(vector<int>&, const int);

// Return the best fitness value in population
double bestFitness(Population&, Map&);

double smallerDistance(Population&, Map&);

pair<int, vector<int>> getBestTour(Population&, Map&);

vector<int> getAllCharges(vector<int>, Map&);

vector<vector<int>> returnEletism(Population&, Map&, const unsigned);

void crossover(vector<int>& , vector<int>&);
#endif