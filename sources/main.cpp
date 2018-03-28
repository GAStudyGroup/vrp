#include <ctime>
#include <fstream>
#include <sstream>
#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "TourUtils.hpp"
#include "GAUtils.hpp"

using namespace std;

const unsigned limitGen{300};
const unsigned popSize{100};

void startGA();

void setParams(){
    ImportData file("libs/more-libs/E-n33-k4.vrp");
    Configs::truckNumber=4;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber)    ;
    Configs::InitialPopmutIterations=100;
    Configs::InitialPopMutRate=70;
    Configs::mutationRate=100;
    Configs::applyWorst=true;
    Configs::fitnessMode=2;
}

int main(){
    srand(time(NULL));
    setParams();
    int start_s=clock();

    startGA();

    int stop_s=clock();
    cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000<< endl;
    return 0;
}

void startGA(){
    unsigned generation{0};

    cout << "Generating first population"<<endl;
    Population pop{popGen(popSize)};
    initialPopApplyMutation(pop);
    Configs::initialBest=pop.getPop()[0].getDist();

    cout << "Starting GA"<<endl;
    cout << pop << endl;

    while(generation < limitGen){
        Configs::currentIteration=generation;
        cout << "Geração: "<< generation<<endl;
        pop = newGeneration(pop);
        // cout << pop.getPop()[0]<<endl;
        generation++;

        // cout << "Generation " << generation<<endl;
        // cout << "Population smaller distance: " << smallerDistance(pop)<<endl;
    }

    cout << pop << endl;
}