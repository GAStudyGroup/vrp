#include <ctime>

#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"

Population newGeneration(Population&);
vector<int> tourGen();

using namespace std;

/* int main(int argc, char* argv[]){
    Mutation mut;
    ImportData file("libs/P-n23-k8.vrp");
    Configs::truckNumber=8;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
    Tour teste= tourGen();
    cout << teste<<endl;
    while(Configs::currentIteration<Configs::maxIterations){
        teste=mut.evaluateMutation(teste.getRoute());
        mut.validateTour(teste.getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId());
        cout << teste<<endl;
       Configs::currentIteration++; 
    }
    return 0;
} */

int main(){
    srand(time(NULL));
    ImportData file("libs/P-n16-k8.vrp");
    Population pop;

    Configs::popSize=2;
    Configs::truckNumber=2;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);

    for(unsigned i=0; i<Configs::popSize; i++){
        Tour t;
        t.getRoute() = tourGen();
        pop.addNewTour(t);
    }

    cout << pop << endl;

    int limit{1}, count{0};
    while(limit > count){
        pop = newGeneration(pop);
        count++;
    }

    cout << pop << endl;

}

Population newGeneration(Population& pop){
    Population newPop;

    for(int i=0; i<Configs::popSize; i++){
        Tour offs;
        offs = GPX2::crossover(pop.getPop()[i], pop.getPop()[(i+1)%Configs::popSize]);
        newPop.addNewTour(offs);
        if(pop.getPop()[i].getDist() > offs.getDist() && pop.getPop()[(i+1)%Configs::popSize].getDist() > offs.getDist()){
            cout << "CROSSOVER " << pop.getPop()[i].getDist() << " " << pop.getPop()[(i+1)%Configs::popSize].getDist() << " " << offs.getDist() << " ";

            cout << "FITNESS " << pop.getPop()[i].getFitness() << " " << pop.getPop()[(i+1)%Configs::popSize].getFitness() << " " << offs.getFitness() << " "<< endl;
        }
    }

    return(newPop);
}

vector<int> tourGen(){
    vector<int> tour;
    int depotId=Configs::customerMap.getDepotId();

        for(auto c : Configs::customerMap.getMap()){
            if(c.getId() != depotId){
                tour.push_back(c.getId());
            }
        }

        std::random_shuffle(tour.begin(), tour.end());

        unsigned tamGambi = tour.size() + Configs::customerMap.getNumberTrucks();
        int i=0, backsGap = (tour.size()+1)/Configs::customerMap.getNumberTrucks();
        while(tour.size() != tamGambi){
            tour.insert(tour.begin()+i, depotId);
            i+=backsGap+1;
        }

       return tour;
}