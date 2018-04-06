#ifndef TOURUTILS_HPP
#define TOURUTILS_HPP

#include <vector>
#include <iostream>
#include <cmath>


#include "CustomerMap.hpp"
#include "Population.hpp"

using std::vector;
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;


// SECTION TO FITNESS CALCULATION
// Main function to calculate fitness, will return the value
// return the charge used of route inside the tour
double getSubCharge(vector<int>&);
// return a distance used in route 
double getSubDistance(vector<int>&);
// return the euclidian distance beetween two points in map
double distance(const int, const int);

#endif