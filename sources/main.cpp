#include <ctime>
#include <fstream>
#include <sstream>
#include "ImportData.hpp"
#include "CustomerMap.hpp"
#include "Configs.hpp"
#include "Tour.hpp"
#include "TourUtils.hpp"
#include "GAUtils.hpp"
#include "TourRepairer.hpp"

using namespace std;

const unsigned limitGen{300};
const unsigned popSize{80};

void startGA();
Tour readFile(string);
void debugGA();

void setParams(){
    //ImportData file("libs/more-libs/E-n33-k4.vrp");
    ImportData file("libs/P-Sets/P-n16-k8.vrp");
    Configs::truckNumber=8;
    Configs::customerMap=CustomerMap(file.getCustomerList(),file.getCapacity(),Configs::truckNumber);
    Configs::InitialPopmutIterations=100;
    Configs::InitialPopMutRate=70;
    Configs::mutationRate=100;
    Configs::applyWorst=true;
    Configs::fitnessMode=2;
}
int main(){
    //srand(time(NULL));
    setParams();
    int start_s=clock();
    Tour original= tourGen();
    cout<<original<<endl;
    Tour novoTour=TourRepairer().repairTour(original);
    cout<<novoTour<<endl;
    // startGA();
    //debugGA();

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
   // cout << pop << endl;

    while(generation < limitGen){
        Configs::currentIteration=generation;
        cout << "Geração: "<< generation<<endl;
        pop = newGeneration(pop);
        cout << pop.getPop()[0]<<endl;
        generation++;
        // cout << "Generation " << generation<<endl;
        // cout << "Population smaller distance: " << smallerDistance(pop)<<endl;
    }

    cout << pop << endl;
}

void debugGA(){
    Tour red, blue;

    red=readFile("Debug/Hamiltonian/quebra1");
    blue=readFile("Debug/Hamiltonian/quebra2");

    Tour offs = crossover(red, blue);
}

Tour readFile(string name){
    ifstream file;

    file.open(name);

    string customer;
    Tour t;
    while(file >> customer){
        t.getRoute().push_back(stoi(customer));
    }
    return(t);
}