#include <cmath>
#include "Fitness.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

using std::ceil;
using std::pow;

//OurFitness
double ourFitness(vector<int>& tour){
    vector<vector<int>> subs = Tour(tour).explodeSubTours();
    double fitness=0;
    
    for(vector<int> sub : subs){
        double chargeUsed = TourUtils::getSubCharge(sub);
        if(chargeUsed <= Configs::customerMap.getTruckCapacity()){
            fitness += subFitness(sub, chargeUsed);
        }else{
            fitness += subFitnessPenalty(sub, chargeUsed);
        }
    }
    return (fitness*100);
}

double subFitness(vector<int>& tour, double& chargeUsed){
    return ((1 /TourUtils::getSubDistance(tour)) * (chargeUsed / Configs::customerMap.getTruckCapacity()));
}

double subFitnessPenalty(vector<int>& tour, double& chargeUsed){
    return ( (1 /TourUtils::getSubDistance(tour)) * -1 * (chargeUsed / Configs::customerMap.getTruckCapacity()));
}

//Advanced Fitness
int calcMnv(){
    double demandSum=0;
    for (auto customer: Configs::customerMap.getMap()){
        demandSum+=customer.getDemand();
    }  
    int mnv=ceil(demandSum/Configs::customerMap.getTruckCapacity());
    return (mnv);
}

double calcAlpha(){
    double divide=pow(((calcMnv()/2)*Configs::customerMap.getTruckCapacity()),2);
    divide=divide*(1/(double)Configs::maxIterations);
    double alpha=(Configs::initialBest/divide);    
    return alpha;
}

double calcPenalty(vector<int>& tour){
    double innerSum=0;
    for(auto charge:Tour(tour).getAllCharges()){
        if(charge>Configs::customerMap.getTruckCapacity()){
            innerSum+=pow((charge - Configs::customerMap.getTruckCapacity()),2);
        }else{
            innerSum+=0;
        }
    }
    double alpha=calcAlpha();
    double rate=((double)Configs::currentIteration/(double)Configs::maxIterations);
    double penalty=rate*alpha*innerSum;
    return 1000*penalty;
}

double advancedFitness(vector<int>& tour){
    double fitness= Tour(tour).getDist();
    fitness+=calcPenalty(tour);
    return (1/fitness)*10000;
}