#include <cmath>
#include "Fitness.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

using std::ceil;
using std::pow;

//OurFitness
double OurFitness::ourFitness(vector<int>& tour){
    vector<vector<int>> subs = Tour(tour).explodeSubTours();
    double fitness=0;
    
    for(vector<int> sub : subs){
        double chargeUsed = TourUtils::getSubCharge(sub);
        if(chargeUsed <= Globals::customerMap.getTruckCapacity()){
            fitness += subFitness(sub, chargeUsed);
        }else{
            fitness += subFitnessPenalty(sub, chargeUsed);
        }
    }
    return (fitness*100);
}

double OurFitness::subFitness(vector<int>& tour, double& chargeUsed){
    return ((1 /TourUtils::getSubDistance(tour)) * (chargeUsed / Globals::customerMap.getTruckCapacity()));
}

double OurFitness::subFitnessPenalty(vector<int>& tour, double& chargeUsed){
    return ( (1 /TourUtils::getSubDistance(tour)) * -1 * (chargeUsed / Globals::customerMap.getTruckCapacity()));
}

//Advanced Fitness

double AdvancedFitness::calcAlpha(){
    double divide=pow(((Globals::customerMap.getMnv()/2)*Globals::customerMap.getTruckCapacity()),2);
    divide=divide*(1/(double)Configs::maxIterations);
    double alpha=(Fitness::initialBest/divide);    
    return alpha;
}

double AdvancedFitness::calcPenalty(vector<int>& tour){
    double innerSum=0;
    for(auto charge:Tour(tour).getAllCharges()){
        if(charge>Globals::customerMap.getTruckCapacity()){
            innerSum+=pow((charge - Globals::customerMap.getTruckCapacity()),2);
        }else{
            innerSum+=0;
        }
    }
    double alpha=calcAlpha();
    double rate=((double)Globals::currentIteration/(double)Configs::maxIterations);
    double penalty=rate*alpha*innerSum;
    return 1000*penalty;
}

double AdvancedFitness::advancedFitness(vector<int>& tour){
    double fitness= Tour(tour).getDist();
    fitness+=calcPenalty(tour);
    return (1/fitness)*10000;
}

// New fitness 

double newFitness::newFitness(vector<int>& tour) {
    double tourDistance{Tour(tour).getDist()};

    return (tourDistance + (calcPenalty(tour) * calcPenaltyMultiplicator(tour)));
}

double newFitness::calcPenalty(vector<int>& tour) {
    double maxDistance{TourUtils::getMaxDistance(tour)};
    int tourSize{ (int) tour.size()};
    return (tourSize * maxDistance);
}

int newFitness::calcPenaltyMultiplicator(vector<int>& tour) {
    vector<int> tourCharges = Tour(tour).getAllCharges();
    // +1 to maintain the 100%/100% < 1
    int capacityDivider{(int)Globals::customerMap.getTruckCapacity() + 1};
    int multiplicator{0};

    for(int charge : tourCharges) {
        multiplicator += std::floor(charge/capacityDivider);
    }
    return (multiplicator);
}