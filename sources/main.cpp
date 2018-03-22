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
#include "GAutils.hpp"
#include "Configs.hpp"

using namespace std;

int iterations{40};
long int generation{0};
double bestFit={-1*numeric_limits<double>::max()};
int stuckCount=0;
Map customerMap;

void initAlg();

//Funções auxiliares para main
void startPop(Population &pop, Map &customerMap);
void iteration(Population &pop,Map &customerMap);
Population newGeneration(Population&);
void printBestTourInfo(Population &pop,Map &customerMap);
void printTourCharges(Population &pop, Map &customerMap);

// Algorithm Arguments
string fileName;

// 1-FileName 2-TruckNumber 3-PopSize  4-MutIterations 5-DisturbFactor 6-ElitismPercentage
int main(int argc, char* argv[]){
    srand(time(NULL));

    if(argc==7){
        try{
            fileName = argv[1];
            Configs::truckNumber = stoi(argv[2]);
            Configs::popSize = stoi(argv[3]);
            Configs::mutIterations = stoi(argv[4]);
            Configs::disturbFactor = stoi(argv[5]);
            double elitPercentage = stof(argv[6]);
            Configs::elitismNumber = (int) Configs::popSize * (elitPercentage/100);
        }catch(invalid_argument &ia){
            cout << "Alguma coisa deu merda"<<endl;
        }
    }else {
        cout << "Arguments not inserted!"<<endl;
    }

    {
        ImportData file(fileName);
        customerMap.setMap(file.getCustomerList());
        customerMap.setNumberTrucks(Configs::truckNumber);
        customerMap.setTruckCapacity(file.getCapacity());
    }

    initAlg();
}

void initAlg(){

    Population pop;
    startPop(pop, customerMap);

    while (generation < iterations){
        iteration(pop, customerMap);
    }

    printBestTourInfo(pop, customerMap);
}


void startPop(Population &pop, Map &customerMap){
    pop = popGen(customerMap, Configs::popSize);
    cout << "First Population"<<endl;
    cout << pop << endl;
    initialPopApplyMutation(pop, Configs::mutIterations,customerMap);
    cout<<"Fim da pop inicial"<<endl;   
}
void iteration(Population &pop,Map &customerMap){
    pop = newGeneration(pop);
    printTourCharges(pop,customerMap);
    if(checkStuck(bestFit,bestFitness(pop,customerMap),stuckCount)){
        stuckCount=0;
        disturb(pop,customerMap, Configs::mutIterations, Configs::disturbFactor);           
   }
}

Population newGeneration(Population& pop){
    Population newPop{pop};
    if(generation%10 == 0){
        cout << "Generation " << generation << " BestFitnessAll " << bestFit << endl << "BestFitnessNow " << bestFitness(pop, customerMap) << " SmallerDistanceNow " << smallerDistance(pop, customerMap) <<endl;
    }    

    cout << "Elitism: " <<  Configs::elitismNumber << endl;
    auto tours = returnEletism(newPop, customerMap, Configs::elitismNumber);

    //Aplicação do crossover
    applyCrossover(newPop,tours);
    // Aplicação da mutação
    applyMutation(newPop,customerMap, Configs::truckNumber);

    if(bestFit < bestFitness(newPop, customerMap)){
        cout << "Fitness Increased " << "\nBefore: " << bestFit << " - After: " << bestFitness(newPop, customerMap)<<endl;
        bestFit = bestFitness(newPop, customerMap);
    }
    generation++;
    return(newPop);
}

void printBestTourInfo(Population &pop, Map &customerMap){
    vector<int> tour = getBestTour(pop, customerMap).second;
    cout << "BestTour: "<<endl;
    coutTour(tour,customerMap);
    printTourCharges(pop,customerMap);
    cout<< "Tour distance: "<<getTourDistance(tour,customerMap)<<endl;
    cout << endl;   
}
void printTourCharges(Population &pop, Map &customerMap){
    vector<int> charges = getAllCharges(getBestTour(pop,customerMap).second, customerMap);
    cout << "TourCharges"<<endl;
    for(int i : charges){
        cout << i << " ";
    }
}