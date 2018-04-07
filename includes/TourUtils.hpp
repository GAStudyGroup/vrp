#ifndef TOURUTILS_HPP
#define TOURUTILS_HPP

#include <vector>
#include <iostream>



#include "CustomerMap.hpp"
#include "Population.hpp"

using std::vector;
//Remover depois e tratar nas classes
using std::cout;
using std::endl;
using std::pair;
using std::make_pair;

// Support function to calculate partial info about Tours
namespace TourUtils{
// return the charge used of route inside the tour
double getSubCharge(vector<int>&);
// return a distance used in route 
double getSubDistance(vector<int>&);
};

#endif