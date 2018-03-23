#include "Fitness.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"
using std::ceil;
using std::pow;

//OurFitness
double ourFitness(vector<int>& tour){ //Buga quando tem dois depósitos no começo
    //printVector(tour,Configs::customerMap); //Coloquei aqui para ajudar o debug
    vector<vector<int>> subs = explodeSubTours(tour, Configs::customerMap.getDepotId());
    double fitness=0;
    
    for(vector<int> sub : subs){
        double chargeUsed = getSubCharge(sub);
        //cout << "Used " << chargeUsed << " / " << Configs::customerMap.getTruckCapacity() << endl;
        if(chargeUsed <= Configs::customerMap.getTruckCapacity()){
            fitness += subFitness(sub, chargeUsed);
        }else{
            fitness += subFitnessPenalty(sub, chargeUsed);
        }
    }
    return (fitness*100);
}

double subFitness(vector<int>& tour, double& chargeUsed){
    /* cout <<endl << "subFitness("<<tour.size()<<")"<<endl;
    for(auto t: tour ) cout << t << " "; */
    return ((1 /getSubDistance(tour)) * (chargeUsed / Configs::customerMap.getTruckCapacity()));
}

double subFitnessPenalty(vector<int>& tour, double& chargeUsed){
    return ( (1 / getSubDistance(tour)) * -1 * (chargeUsed / Configs::customerMap.getTruckCapacity()));
}

//Advanced Fitness
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

double calcPenalty(vector<int>& tour){
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
    return penalty;
}

double advancedFitness(vector<int>& tour){
    double fitness= getTourDistance(tour);
    fitness+=calcPenalty(tour);
    return (1/fitness)*10000;
}