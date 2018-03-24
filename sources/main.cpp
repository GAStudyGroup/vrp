#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"


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
    ImportData file("testesManuais/batata1.vrp");

    Configs::truckNumber=3;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
    Tour test1, test2;

    test1.getRoute().push_back(0); 
    test1.getRoute().push_back(9);
    test1.getRoute().push_back(4);
    test1.getRoute().push_back(3);
    test1.getRoute().push_back(2);
    test1.getRoute().push_back(0);
    test1.getRoute().push_back(7);
    test1.getRoute().push_back(6);
    test1.getRoute().push_back(0);
    test1.getRoute().push_back(5);
    test1.getRoute().push_back(10);
    test1.getRoute().push_back(1);
    test1.getRoute().push_back(8);

    test2.getRoute().push_back(3); 
    test2.getRoute().push_back(9);
    test2.getRoute().push_back(0);
    test2.getRoute().push_back(1);
    test2.getRoute().push_back(8);
    test2.getRoute().push_back(10);
    test2.getRoute().push_back(5);
    test2.getRoute().push_back(0);
    test2.getRoute().push_back(7);
    test2.getRoute().push_back(6);
    test2.getRoute().push_back(2);
    test2.getRoute().push_back(4);
    test2.getRoute().push_back(0);
    
    cout << test1<<endl;
    cout << test2<<endl;

    Tour t = GPX2::crossover(test1, test2);

    cout << t;
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