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
ImportData file("libs/X-n101-k25.vrp");
Configs::truckNumber=25;
Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
Configs::InitialPopmutIterations=2;
Configs::InitialPopMutRate=100;
Configs::mutationRate=100;
Configs::applyWorst=false;
}

int main(int argc, char* argv[]){
    //srand(time(NULL));
    setParams();
    int start_s=clock();
    Population pop=popGen(20);
    //Insert code here
    initialPopApplyMutation(pop);
    while(1){
        pop=newGeneration(pop);
    }
    int stop_s=clock();
    cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000<< endl;
    return 0;
} 
