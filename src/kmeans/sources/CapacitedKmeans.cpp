#include "CapacitedKmeans.hpp"
#include "Kmeans.hpp"
#include "Distance.hpp"
#include "Configs.hpp"
#include<algorithm>
bool CapacitedCentroidCalc::compareCentroids(vector<Structure::Centroid> before,
vector<Structure::Centroid> after){
    if(before.size()!=after.size()){
        return false;
    }
    for(unsigned i=0;i<before.size();i++){
        if(!CentroidCalc::compareDoubles(before[i].x,after[i].x)
         || !CentroidCalc::compareDoubles(before[i].y,after[i].y)){
            return false;
        }
    }
    return true;
}
vector<Structure::Centroid> CapacitedCentroidCalc::getAllCentroids(Tour& tour){
    vector<Structure::Centroid> centroids;    
    int id=0;
    for(auto route: tour.explodeSubTours()){
        Structure::Centroid aux;
        aux.id=id;
        auto centroidValue=CentroidCalc::calcRouteCentroid(route);
        aux.x=centroidValue.first;
        aux.y=centroidValue.second;
        centroids.push_back(aux);
        id++;
    }
    return centroids;
}
//Returns the ids of the centroids sorted(ASC) by the distance that they are from the customer
vector<int> CapacitedClassifier::getNearestCentroids(vector<Structure::Centroid> centroids,
int customer){
    double customerX=Globals::customerMap.getCustomer(customer).getX();;
    double customerY=Globals::customerMap.getCustomer(customer).getY();;
    
    std::sort(centroids.begin(),centroids.end(),[customerX,customerY](Structure::Centroid a,
    Structure::Centroid b){
        return distance(a.x,a.y,customerX,customerY) 
        < distance(b.x,b.y,customerX,customerY) ;
    });
    vector<int> ids;
    for(auto centroid:centroids){
        ids.push_back(centroid.id);
    }
    return ids;
}