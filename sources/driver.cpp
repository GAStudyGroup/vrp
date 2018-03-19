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

using namespace std;

int iterations{40};
long int generation{0};
double bestFit={-1*numeric_limits<double>::max()};
const int truckNumber{2};
const int popSize{60};
const int mutIterations{50};
const int disturbFactor{5};
const double elitPercentage{0.25};
int elitN=(int) popSize*elitPercentage;
int stuckCount=0;

ImportData file("libs/P-n22-k2.vrp");
Map customerMap(file.getCustomerList(), file.getCapacity(), truckNumber);

//Funções auxiliares para main
void startPop(Population &pop, Map &customerMap);
void iteration(Population &pop,Map &customerMap);
Population newGeneration(Population&);
void printBestTourInfo(Population &pop,Map &customerMap);
void printTourCharges(Population &pop, Map &customerMap);

int main(){   
    srand(time(NULL));
    int count{0};
    //Inicialização da população
    Population pop;
    startPop(pop,customerMap);    
    //Loop do genético
    while(count < iterations){
        iteration(pop,customerMap);
        count ++;
    }
    //Imprimindo as informações do melhor tour após todas as iterações
    printBestTourInfo(pop,customerMap);
    //Imprime toda a população antes de encerrar o algoritmo
    cout << "\n\n\nPOPULATION"<<endl<<pop << endl;
}


void startPop(Population &pop, Map &customerMap){
    pop = popGen(customerMap, popSize);
    cout << "First Population"<<endl;
    cout << pop << endl;
    initialPopApplyMutation(pop,mutIterations,customerMap);
    cout<<"Fim da pop inicial"<<endl;   
}
void iteration(Population &pop,Map &customerMap){
    pop = newGeneration(pop);
    printTourCharges(pop,customerMap);
    if(checkStuck(bestFit,bestFitness(pop,customerMap),stuckCount)){
        stuckCount=0;
        disturb(pop,customerMap,mutIterations,disturbFactor);           
   }
}

Population newGeneration(Population& pop){
    Population newPop{pop};
    if(generation%10 == 0){
        cout << "Generation " << generation << " BestFitnessAll " << bestFit << endl << "BestFitnessNow " << bestFitness(pop, customerMap) << " SmallerDistanceNow " << smallerDistance(pop, customerMap) <<endl;
    }    
    cout << "Elitism: " << elitN << endl;
    auto tours = returnEletism(newPop, customerMap, elitN);
    //Aplicação do crossover
    applyCrossover(newPop,tours);
    // Aplicação da mutação
    applyMutation(newPop,customerMap,truckNumber);
    generation++;
    if(bestFit < bestFitness(newPop, customerMap)){
        cout << "Fitness Increased " << "\nBefore: " << bestFit << " - After: " << bestFitness(newPop, customerMap)<<endl;
        bestFit = bestFitness(newPop, customerMap);
    }
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