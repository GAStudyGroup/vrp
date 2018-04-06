#include "TourUtils.hpp"
#include "Configs.hpp"


double getSubCharge(vector<int>& tour){
    double charge=0;

    for(int c : tour){
        charge += Configs::customerMap.getCustomer(c).getDemand();
    }
    return (charge);
}

double getSubDistance(vector<int>& tour){
    double dist=0;

    dist += distance(tour[0], Configs::customerMap.getDepotId()); // depot to first customer
    dist += distance(tour[tour.size()-1], Configs::customerMap.getDepotId()); // last customer to depot
    for(unsigned i=0; i<tour.size()-1; i++){
        dist += distance(tour[i], tour[i+1]);
    }
    return (dist);
}

double distance(const int a, const int b){
    //return(std::round(std::sqrt(std::pow(Configs::customerMap.getCustomer(a).getX() - Configs::customerMap.getCustomer(b).getX(), 2) + pow(Configs::customerMap.getCustomer(a).getY() - Configs::customerMap.getCustomer(b).getY(), 2))));
    return(std::sqrt(std::pow(Configs::customerMap.getCustomer(a).getX() - Configs::customerMap.getCustomer(b).getX(), 2) + pow(Configs::customerMap.getCustomer(a).getY() - Configs::customerMap.getCustomer(b).getY(), 2)));
}
