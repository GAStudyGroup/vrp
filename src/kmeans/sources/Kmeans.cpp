#include "Kmeans.hpp"
#include "Configs.hpp"
#include "Distance.hpp"

#include <limits>
//Centoid calculations
using std::cout;
using std::endl;
pair<double,double> CentroidCalc::calcRouteCentroid(vector<int>& route){
    double xCenter=0;
    double yCenter=0;

    for(int customer:route){
        xCenter+=Globals::customerMap.getCustomer(customer).getX();
        yCenter+=Globals::customerMap.getCustomer(customer).getY();
    }
    
    xCenter=(xCenter/route.size());
    yCenter=(yCenter/route.size());

    return std::make_pair(xCenter,yCenter);
}

vector<pair<double,double>> CentroidCalc::getAllCentroids(Tour& tour){
    vector<pair<double,double>> centroids;
    for(auto route: tour.explodeSubTours()){
        centroids.push_back(calcRouteCentroid(route));
    }
    return centroids;
}
//Find the nearest centroid
int Classifier::findNearestCentroid(vector<pair<double,double>> centroids,int customer){
    //Calc all distances
    vector<double> centroidDistances;
    for(auto centroid: centroids){
        centroidDistances.push_back(distance(centroid.first,centroid.second,
            Globals::customerMap.getCustomer(customer).getX(),
            Globals::customerMap.getCustomer(customer).getY()));
    }
    // for(auto distance:centroidDistances){
    //     cout<<distance<<endl;
    // }
    //Find the nearest distance
    double minDistance = std::numeric_limits<double>::max();
    double minDistanceCentroid=0;
    for(int i=0;i<centroidDistances.size();i++){
        if(centroidDistances[i]<minDistance){
            minDistance=centroidDistances[i];
            minDistanceCentroid=i;
        }
    }
    return minDistanceCentroid;
}