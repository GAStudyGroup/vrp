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
const unsigned popSize{30};

void startGA();

void setParams(){
    srand(time(NULL));
    ImportData file("libs/more-libs/E-n101-k14.vrp");
    Configs::truckNumber=14;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber)    ;
    Configs::InitialPopmutIterations=2;
    Configs::InitialPopMutRate=100;
    Configs::mutationRate=100;
    Configs::applyWorst=false;
}

int main(){
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

    cout << "Starting GA"<<endl;
    cout << pop << endl;

    while(generation < limitGen){
        pop = newGeneration(pop);
        generation++;

        cout << "Generation " << generation<<endl;
        cout << "Population smaller distance: " << smallerDistance(pop)<<endl;
    }

    cout << pop << endl;
}