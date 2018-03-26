#include <ctime>
#include <vector>
#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "GAUtils.hpp"

#include "Mutation.hpp"

using namespace std;
void setParams(){
ImportData file("libs/teste.vrp");
Configs::truckNumber=2;
Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
Configs::InitialPopmutIterations=2 ;
Configs::InitialPopMutRate=100;
Configs::mutationRate=100;
Configs::applyWorst=false;
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    setParams();
    vector<int> t1v;
    vector<int> t2v;
    t1v.push_back(2);
    t1v.push_back(1);
    t1v.push_back(3);
    t1v.push_back(5);
    t1v.push_back(4);
    
    t2v.push_back(2);
    t2v.push_back(1);
    t2v.push_back(3);
    t2v.push_back(4);
    t2v.push_back(5);
    Tour t1= Tour(t1v);
    Tour t2= Tour(t2v);
    if(t1!=t2){
      cout<<"Diferentes"<<endl;
    }else{
        cout<<"Iguais"<<endl;
    }
    /* Population pop=popGen(20);
    initialPopApplyMutation(pop);
    pop.sortPop();

    for(int count=0;count<1000;count++){
       // cout<<pop.getPop()[0]<<endl;
        pop=newGeneration(pop);
        ///cout <<pop.getPop()[0]<<endl;
    } */
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


