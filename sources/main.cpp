#include <ctime>

#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "GAUtils.hpp"
#include "Mutation.hpp"
#include "GPX2.hpp"
#include "HamiltonianCycle.hpp"

using namespace std;


void setParams(){
ImportData file("libs/P-n16-k8.vrp");
Configs::truckNumber=8;
Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
Configs::InitialPopmutIterations=10;
}

Tour crossover(Tour&, Tour&);
/* int main(int argc, char* argv[]){
    //srand(time(NULL));
    setParams();
    Population pop=popGen(30);
    cout<<pop<<endl;
    initialPopApplyMutation(pop);
    cout<<pop<<endl;
    return 0;
}  */

int main(){  // MAIN PARA TESTES, NAO MEXER NESSA MERDA (elcio)
    //srand(time(NULL));
    ImportData file("testesManuaisDeValidação/mapa1.vrpTest");
    Population pop;

    Configs::truckNumber=3;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);

    Tour red, blue;

    red.getRoute().push_back(1);
    red.getRoute().push_back(2);
    red.getRoute().push_back(3);
    red.getRoute().push_back(4);
    red.getRoute().push_back(1);
    red.getRoute().push_back(5);
    red.getRoute().push_back(6);
    red.getRoute().push_back(7);
    red.getRoute().push_back(1);
    red.getRoute().push_back(8);
    red.getRoute().push_back(9);
    red.getRoute().push_back(10);

    blue.getRoute().push_back(1);
    blue.getRoute().push_back(5);
    blue.getRoute().push_back(7);
    blue.getRoute().push_back(6);
    blue.getRoute().push_back(1);
    blue.getRoute().push_back(8);
    blue.getRoute().push_back(9);
    blue.getRoute().push_back(2);
    blue.getRoute().push_back(1);
    blue.getRoute().push_back(10);
    blue.getRoute().push_back(3);
    blue.getRoute().push_back(4);

    
    cout << "Red"<< " ";
    cout << red;
    cout << "Blue"<< " ";
    cout << blue;

    HamiltonianCycle::parentsHamiltonian parents =  HamiltonianCycle::toHamiltonianCycle(red, blue);

}

// int main(){
//     srand(time(NULL));
//     ImportData file("libs/P-n16-k8.vrp");
//     Population pop;

//     Configs::popSize=2;
//     Configs::truckNumber=2;
//     Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);

//     for(unsigned i=0; i<Configs::popSize; i++){
//         Tour t;
//         t.getRoute() = tourGen();
//         pop.addNewTour(t);
//     }

//     cout << pop << endl;

//     int limit{1}, count{0};
//     while(limit > count){
//         pop = newGeneration(pop);
//         count++;
//     }

//     cout << pop << endl;

// }


