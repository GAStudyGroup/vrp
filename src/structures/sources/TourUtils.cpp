#include "TourUtils.hpp"
#include "Configs.hpp"
#include "Distance.hpp"

double TourUtils::getSubCharge(vector<int>& tour){
    double charge=0;

    for(int c : tour){
        charge += Globals::customerMap.getCustomer(c).getDemand();
    }
    return (charge);
}

double TourUtils::getSubDistance(vector<int>& tour){
    double dist=0;

    dist += distance(tour[0], Globals::customerMap.getDepotId()); // depot to first customer
    dist += distance(tour[tour.size()-1], Globals::customerMap.getDepotId()); // last customer to depot
    for(unsigned i=0; i<tour.size()-1; i++){
        dist += distance(tour[i], tour[i+1]);
    }
    return (dist);
}

