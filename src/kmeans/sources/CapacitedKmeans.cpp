#include "CapacitedKmeans.hpp"
#include "Kmeans.hpp"
#include "Distance.hpp"
#include "Configs.hpp"
#include "TourRepairer.hpp"
#include<algorithm>
using std::cout;
using std::endl;
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

Tour CapacitedClassifier::capacitedKmeansBasic(Tour& originalTour,
vector<Structure::Centroid> centroids){
    //Starting  Iteratons
    //const double maxCargo= Globals::customerMap.getTruckCapacity();
    const int depotId=Globals::customerMap.getDepotId();
    vector<int> tour=originalTour.getRoute();
    vector<vector<int>> routes;
    vector<int> startAux;
    for(unsigned i=0;i<centroids.size();i++){
        routes.push_back(startAux);
    }
    vector<int> unassigned;
    for(int customer:tour){
        if(customer!=depotId){
            unassigned.push_back(customer);
        }
    }
    //Classification
    for(int customer:tour){
        for(int routeId: CapacitedClassifier::getNearestCentroids(centroids,customer)){
            if(customer==depotId){
                break;
            }
            // std::cout<<"Customer: "<<customer<<std::endl;
            if(!TourRepairer::willOverload(routes[routeId],customer)){
                // std::cout<<"Customer: "<<customer<< " Inserido"<<std::endl;
                unassigned.erase(std::find(unassigned.begin(),
                unassigned.end(),customer));
                routes[routeId].push_back(customer);
                break;
            }
            
        }
    }
    if(unassigned.size()>0){
        //Assign to nearest centroid
        for(auto un: unassigned){
            routes[CapacitedClassifier::getNearestCentroids(centroids,un)[0]]
            .push_back(un);
        }
    }
    //Insert a depot in the beginning of each route
    for(auto &route : routes){
        route.emplace(route.begin(),depotId);
    }
    
    //Insert empty routes
    int emptyTotal=Configs::truckNumber -  centroids.size();
    vector<int> emptyAux;
    emptyAux.push_back(depotId);
    for(int i=0;i<emptyTotal;i++){
        routes.push_back(emptyAux);
    }
    
    //Rebuild and return tour
    return TourRepairer::tourRebuilder(routes);        
}

Tour CapacitedKmeans::run(Tour& originalTour){
    if(originalTour.explodeSubTours().size()>=(unsigned)Globals::customerMap.getMnv()){
        Tour tour=originalTour;
        auto centroids = CapacitedCentroidCalc::getAllCentroids(originalTour);
        vector<Structure::Centroid> centroidsLast;
        int safeMeasure=0;
        while(!CapacitedCentroidCalc::compareCentroids(centroids,centroidsLast)
        && safeMeasure!= KmeansCfg::KmeansIterations){
            centroidsLast=CapacitedCentroidCalc::getAllCentroids(tour);
            tour=CapacitedClassifier::capacitedKmeansBasic(tour,centroids);
            centroids=CapacitedCentroidCalc::getAllCentroids(tour);
            safeMeasure++;
            // cout<<"Rodando:"<<safeMeasure<<endl;
        }
        return tour;
    }else{
        return originalTour;
    }
}