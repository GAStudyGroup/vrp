#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "Mutation.hpp"
#include "MutationUtils.hpp"
vector<int> tourGen();

int main(int argc, char* argv[]){
    ImportData file("libs/P-n16-k8.vrp");
    Configs::truckNumber=10;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
    Tour teste= tourGen();
    cout << teste<<endl;
    while(Configs::currentIteration<Configs::maxIterations){
        teste=Mutation().evaluateMutation(teste.getRoute());
        validateTour(teste.getRoute(),Configs::truckNumber,Configs::customerMap.getDepotId());
        cout << teste<<endl;
       Configs::currentIteration++; 
    }
    return 0;
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