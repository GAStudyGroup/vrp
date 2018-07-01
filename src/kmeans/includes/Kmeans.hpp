#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <vector>
#include "Tour.hpp"
using std::vector;
using std::pair;

//X and Y from centroid point
namespace CentroidCalc{
    pair<double,double> calcRouteCentroid(vector<int>&);
    vector<pair<double,double>> getAllCentroids(Tour&);
}
#endif