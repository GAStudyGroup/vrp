#include <ctime>

#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "GAUtils.hpp"

#include "Mutation.hpp"

using namespace std;
void setParams(){
ImportData file("libs/P-n16-k8.vrp");
Configs::truckNumber=8;
Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
Configs::InitialPopmutIterations=10;
}

int main(int argc, char* argv[]){
    //srand(time(NULL));
    setParams();
    Population pop=popGen(30);
    cout<<pop<<endl;
    initialPopApplyMutation(pop);
    cout<<pop<<endl;
    return 0;
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


