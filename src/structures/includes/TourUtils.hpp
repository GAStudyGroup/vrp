#ifndef TOURUTILS_HPP
#define TOURUTILS_HPP

#include <vector>
#include <iostream>
#include <limits>

using std::vector;

// Support function to calculate partial info about Tours
namespace TourUtils{
    // return the charge used of route inside the tour
    double getSubCharge(vector<int>&);
    // return a distance used in route 
    double getSubDistance(vector<int>&);
    // return the max distance beetween to customers in tour
    double getMaxDistance(vector<int>&);
};

#endif