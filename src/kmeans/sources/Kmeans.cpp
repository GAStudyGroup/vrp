#include "Kmeans.hpp"
#include "Configs.hpp"

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