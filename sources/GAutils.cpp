#include "GAutils.hpp"

vector<int> tourGen(Map& map){
    vector<int> tour;
    int depotId=map.getDepotId();

        for(auto c : map.getMap()){
            if(c.getId() != depotId){
                tour.push_back(c.getId());
            }
        }

        std::random_shuffle(tour.begin(), tour.end());

        unsigned tamGambi = tour.size() + map.getNumberTrucks();
        int i=0, backsGap = (tour.size()+1)/map.getNumberTrucks();
        while(tour.size() != tamGambi){
            tour.insert(tour.begin()+i, depotId);
            i+=backsGap+1;
        }

       return tour;
}

Population popGen(Map& map, int popSize){
    Population pop;
    for(int x=0; x<popSize; x++){
        vector<int> tour= tourGen(map);
        pop.addNewTour(tour);
    }
    return (pop);
}

void coutTour(vector<int> tour, Map& map){
    cout << "Tour: ";
    for(int i : tour){
        cout << i << " ";
    }
    cout << "\tFitness: " << getTourFitness(tour, map)<<endl;
}

bool checkStuck(double bestFitK, double lastBestFit,int &stuckCount){
    
    if(bestFitK==lastBestFit){
        stuckCount++;
    }
    if(stuckCount==1){
        cout<<"STUCKKKK"<<endl;
        return true;
    }
    return false;
}
void applyCrossover(Population &pop,vector<vector<int>> &tours){
        for(unsigned i=0; i<pop.getPop().size(); i++){
        if(i==pop.getPop().size()-1){
            crossover(pop.getPop()[i], pop.getPop()[0]);
        }else{
            crossover(pop.getPop()[i], pop.getPop()[i+1]);
        }
    }    
    for(vector<int> i : tours){
        pop.getPop().push_back(i);
    }
}

void applyMutation(Population &pop,Map &customerMap, int truckNumber){
       for(unsigned i=0;i<pop.getPop().size(); i++){
        pop.getPop()[i]=Mutation(pop.getPop()[i],customerMap);
        if(!validateTour(pop.getPop()[i],truckNumber,customerMap.getDepotId())){
            exit(-1);
        }
        //printVector(pop.getPop()[i],customerMap);
    } 
}

void initialPopApplyMutation(Population &pop,int mutIterations,Map &customerMap){
    for(int i=0;i<mutIterations;i++){
        unsigned int popAux=0;
        for(auto tour:pop.getPop()){
            tour=Mutation(tour,customerMap);
            popAux++;
            if(popAux==pop.getPop().size()/2){
                break;
            }
        }
    }
}

void disturb(Population &pop, Map &customerMap, int mutIterations,int disturbFactor){
    for(unsigned int i=0;i<pop.getPop().size()/disturbFactor;i++){
        vector<int> aux =tourGen(customerMap);
        for(int j=0;j<mutIterations/4;j++){
            pop.getPop()[i]=Mutation(aux,customerMap);
        }    
    }
}
