#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"


vector<int> tourGen();

using namespace std;

int main(int argc, char* argv[]){
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
}

/* int main(){
    ImportData file("testesManuais/batata1.vrp");

    Configs::truckNumber=3;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
    Tour test1, test2;
} */

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