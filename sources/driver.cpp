#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <limits>

#include "ImportData.hpp"
#include "Map.hpp"
#include "Utils.hpp"
#include "Population.hpp"
#include "Mutation.hpp"
#include "MutationUtils.hpp"

using namespace std;

int iterations{20000};
long int generation{1};
double bestFit={-1*numeric_limits<double>::max()};
ImportData file("libs/P-n51-k10.vrp");
const int truckNumber=20;
Map customerMap(file.getCustomerList(), file.getCapacity(), truckNumber);

// APENAS PARA DESENVOLVIMENTO BÁSICO
Population popGen(Map&, int);
void crossover(vector<int>&, vector<int>&);
Population newGeneration(Population&);

int main(){   
    srand(time(NULL));
    int count{0};
    Population pop;
    pop = popGen(customerMap, 1);
    cout << "First Population"<<endl;
    cout << pop << endl;
    /*printVector(pop.getPop()[0],customerMap);
    pop.getPop()[0]=Mutation(pop.getPop()[0],customerMap);
    validateTour(pop.getPop()[0],8);
    printVector(pop.getPop()[0],customerMap);*/
    while(count < iterations){
        pop = newGeneration(pop);
        count++;
    }
    pop = newGeneration(pop); 
    cout << "\nBest " << bestFitness(pop, customerMap)<<endl;
    vector<int> tour = getBestTour(pop, customerMap);
    cout << "BestTour"<<endl;
    for(int i : tour){
        cout << i << " ";
    }
    cout << endl;
    vector<int> charges = getAllCharges(tour, customerMap);
    cout << "TourCharges"<<endl;
    for(int i : charges){
        cout << i << " ";
    }
    cout << endl;
}

Population popGen(Map& map, int popSize){
    int depotId = map.getDepotId();
    Population pop;

    for(int x=0; x<popSize; x++){
        vector<int> tour;

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

        pop.addNewTour(tour);
    }
    return (pop);
}

void crossover(vector<int>& parent1, vector<int>& parent2){
    vector<int> tmp1{parent1}, tmp2{parent2};
    vector<int> aux1, aux2;
    int cutPoint{parent1.size()/2};
    int infLimit{parent1.size()};
    int ctrl1{0}, ctrl2{0};

    if(parent1.size() != parent2.size()){
        cout << "AGORA TUDO FAZ SENTIDO, TODAS AS PEÇAS SE ENCAIXARAM MUAHAAHAHAHAHAH" << endl;
        cutPoint = ((parent1.size() + parent2.size())/4);
        if(parent1.size() > parent2.size()){
            infLimit = parent2.size();
        }
        if(infLimit < cutPoint){
            cutPoint = infLimit;
        }
    }

    for(unsigned i=cutPoint; i<infLimit; i++){
        swap(tmp1, i, findElement(tmp1, parent2[i]));
        tmp1[i] = parent2[i];
        swap(tmp2, i, findElement(tmp2, parent1[i]));
        tmp2[i] = parent1[i];
    }
    parent1 = tmp1;
    parent2 = tmp2;    
}

Population newGeneration(Population& pop){
    Population newPop{pop};
    if(generation%10 == 0){
        cout << "Generation " << generation << " BestFitnessAll " << bestFit << endl << "BestFitnessNow " << bestFitness(pop, customerMap) << " SmallerDistanceNow " << smallerDistance(pop, customerMap) <<endl;
    }
     /*
    for(unsigned i=0; i<newPop.getPop().size(); i++){
        if(i==newPop.getPop().size()-1){
            crossover(newPop.getPop()[i], newPop.getPop()[0]);
        }else{
            crossover(newPop.getPop()[i], newPop.getPop()[i+1]);
        }
    }*/
    //Mutação aqui
    for(unsigned i=0;i<newPop.getPop().size(); i++){
        newPop.getPop()[i]=Mutation(newPop.getPop()[i],customerMap);
        if(!validateTour(newPop.getPop()[i],truckNumber,customerMap.getDepotId())){
            exit(-1);
        }
        //printVector(newPop.getPop()[i],customerMap);
    }
    generation++;
    if(bestFit < bestFitness(newPop, customerMap)){
        cout << "Fitness Increased " << "\nBefore: " << bestFit << " - After: " << bestFitness(newPop, customerMap)<<endl;
        bestFit = bestFitness(newPop, customerMap);
    }
    return(newPop);
}
