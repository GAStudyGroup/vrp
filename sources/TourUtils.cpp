#include "TourUtils.hpp"
#include "Configs.hpp"



double getTourDistance(vector<int>& tour){
    vector<vector<int>> subs = explodeSubTours(tour, Configs::customerMap.getDepotId());
    double distance=0;

    for(vector<int> sub : subs){
        distance += getSubDistance(sub);
    }
    return(distance);
}


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

vector<vector<int>> explodeSubTours(vector<int> tour, int depotId){
    vector<vector<int>> tours;
    vector<int> sub;

    while(tour.size()){
        if(tour.back() == depotId || tour.front() == depotId){
            if(tour.front() == depotId){
                tour.erase(tour.begin());
            }
            while(tour.front() != depotId && tour.size() != 0){
                sub.push_back(tour.front());
                tour.erase(tour.begin());
            }
            if(!sub.empty()){
                tours.push_back(sub);
                sub.clear();
            }
        }else{
            while(tour.back() != depotId){
                sub.insert(sub.begin(), tour.back());
                tour.erase(tour.end()-1);
            }
            while(tour.front() != depotId){
                sub.push_back(tour.front());
                tour.erase(tour.begin());
            }
            tour.erase(tour.begin()); // remove depotId from front
            tours.push_back(sub);
            sub.clear();
        }
    }
    if(tours.back().empty()){
        tours.erase(tours.end()-1);
    }
    return(tours);
}

double smallerDistance(Population& pop){
    double best{std::numeric_limits<double>::max()};
    for(auto tour : pop.getPop()){
        double distance = getTourDistance(tour);
        if(distance < best){
            best = distance;
        }
    }
    return(best);
}

vector<int> getAllCharges(vector<int> tour){
    vector<vector<int>> subs = explodeSubTours(tour, Configs::customerMap.getDepotId());
    vector<int> charges;

    for(vector<int> sub : subs){
        charges.push_back((int)getSubCharge(sub));
    }
    return(charges);
}
