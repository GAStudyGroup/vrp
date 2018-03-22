#include "Configs.hpp"
#include "Utils.hpp"
using std::ceil;
using std::pow;
int calcMnv(){
    double demandSum=0;
    for (auto customer: Configs::customerMap.map){
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

double calcPenalty(vector<int> tour){
    double innerSum=0;
    for(auto charge:getAllCharges(tour)){
        if(charge>Configs::customerMap.getTruckCapacity()){
            innerSum+=pow((charge - Configs::customerMap.getTruckCapacity()),2);
        }else{
            innerSum+=0;
        }
    }
    double alpha=calcAlpha();
    double rate=((double)Configs::currentIteration/(double)Configs::maxIterations);
    double penalty=rate*alpha*innerSum;
    return 100*penalty;
}

double advancedFitness(vector<int> tour){
    double fitness= getTourDistance(tour);
    fitness+=calcPenalty(tour);
    return (1/fitness)*10000;
}