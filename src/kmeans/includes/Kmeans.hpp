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
//Given a customer, returns the nearest centroid from it
namespace Classifier{
    int findNearestCentroid(vector<pair<double,double>> centroids,int customer);
}
#endif