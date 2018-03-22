#include "Utils.hpp"
#include "Mutation.hpp"
double getTourFitness(vector<int>& tour, Map& map){ //
    //printVector(tour,map); //Coloquei aqui para ajudar o debug
    vector<vector<int>> subs = explodeSubTours(tour, map.getDepotId());
    double fitness=0;
    /* cout  << endl << "TourComplete"<<endl;
    for(auto t : tour) cout << t << " "; */
    for(vector<int> sub : subs){
        double chargeUsed = getSubCharge(sub, map);
        //cout << "Used " << chargeUsed << " / " << map.getTruckCapacity() << endl;
        if(chargeUsed <= map.getTruckCapacity()){
            fitness += subFitness(sub, chargeUsed, map);
        }else{
            fitness += 10*subFitnessPenalty(sub, chargeUsed, map);
        }
    }
    return (fitness*100);
}

double getTourDistance(vector<int>& tour, Map& map){
    vector<vector<int>> subs = explodeSubTours(tour, map.getDepotId());
    double distance=0;

    for(vector<int> sub : subs){
        distance += getSubDistance(sub, map);
    }
    return(distance);
}

double subFitness(vector<int>& tour, double& chargeUsed, Map& map){
    /* cout <<endl << "subFitness("<<tour.size()<<")"<<endl;
    for(auto t: tour ) cout << t << " "; */
    return ((1 /getSubDistance(tour, map)) * (chargeUsed / map.getTruckCapacity()));
}

double subFitnessPenalty(vector<int>& tour, double& chargeUsed, Map& map){
    return ( (1 / getSubDistance(tour, map)) * -1 * (chargeUsed / map.getTruckCapacity()));
}

double getSubCharge(vector<int>& tour, Map& map){
    double charge=0;

    for(int c : tour){
        charge += map.getCustomer(c).getDemand();
    }
    return (charge);
}

double getSubDistance(vector<int>& tour, Map& map){
    double dist=0;

    dist += distance(tour[0], map.getDepotId(), map); // depot to first customer
    dist += distance(tour[tour.size()-1], map.getDepotId(), map); // last customer to depot
    for(unsigned i=0; i<tour.size()-1; i++){
        dist += distance(tour[i], tour[i+1], map);
    }
    return (dist);
}

double distance(const int a, const int b, Map& map){
    return(std::round(std::sqrt(std::pow(map.getCustomer(a).getX() - map.getCustomer(b).getX(), 2) + pow(map.getCustomer(a).getY() - map.getCustomer(b).getY(), 2))));
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

void swap(vector<int>& vector, const int a, const int b){
    int tmp = vector[a];
    vector[a]=vector[b];
    vector[b]=tmp;
}

int findElement(vector<int>& vector, const int element){
    for(unsigned i=0; i<vector.size(); i++){
        if(vector[i] == element) return (i);
    }
    return(-1);
}

double bestFitness(Population& pop, Map& map){
    double best{-1*std::numeric_limits<double>::max()};
    for(auto tour : pop.getPop()){
        double tourFitness = getTourFitness(tour, map);
        if(tourFitness > best){
            best = tourFitness;
        }
    }
    return(best);
}

double smallerDistance(Population& pop, Map& map){
    double best{std::numeric_limits<double>::max()};
    for(auto tour : pop.getPop()){
        double distance = getTourDistance(tour, map);
        if(distance < best){
            best = distance;
        }
    }
    return(best);
}

pair<int, vector<int>> getBestTour(Population& pop, Map& map) {
    double best{-1*std::numeric_limits<double>::max()};
    int bestPos=-1;

    for(unsigned int i=0; i<pop.getPop().size(); i++){
        double fitness = getTourFitness(pop.getPop()[i], map);
        if(fitness > best){
            best = fitness;
            bestPos = i;
        }
    }
    return(make_pair(bestPos, pop.getPop()[bestPos]));
}

vector<int> getAllCharges(vector<int> tour, Map& map){
    vector<vector<int>> subs = explodeSubTours(tour, map.getDepotId());
    vector<int> charges;

    for(vector<int> sub : subs){
        charges.push_back((int)getSubCharge(sub, map));
    }
    return(charges);
}