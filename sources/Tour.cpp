#include "Tour.hpp"
#include "TourUtils.hpp"
#include "Configs.hpp"
#include "Fitness.hpp"
#include <iostream>

Tour::Tour() {}

Tour::Tour(vector<int> route)
    : route(route)
{
}

Tour::Tour(vector<Customer> route){
    for(Customer c : route){
        this->route.push_back(c.getId());
    }
}
Tour::Tour(vector<string> route){
    for(string c : route){
        this->route.push_back(stoi(c));
    }
}

void Tour::setRoute(vector<int> route)
{
    this->route = route;
}

vector<int>& Tour::getRoute()
{
    return route;
}

double Tour::getFitness()
{ 
    switch (Configs::fitnessMode){
        case 1:
            return ourFitness(route);
        break;
        case 2:
            return advancedFitness(route);
        break;
        default:
            return -1;
        break;
    }
}

double Tour::getDist(){
    return getTourDistance(route);
}

ostream& operator<<(ostream& output, Tour& t)
{ // Overload de operador para impressão da população
    output<<"Tour: \n";
    for (int c : t.getRoute()) {
        output << c << "\n" ;
    }
    // output << "\nFitness: " << t.getFitness();
    // output << "\nDistance: "<<t.getDist();
    // output << "\nCharges:";
    // for(auto charge:getAllCharges(t.getRoute())){
    //      output<<" "<<charge;
    // }
    output<<"\nSubtours: \n";
    for(auto subtour: explodeSubTours(t.getRoute(),Configs::customerMap.getDepotId())){
        output<< "| ";
        for(auto customer:subtour){
            output<< customer <<" ";
        }
        output<<"| \n";
    }
    output<< "\nSize:"<< t.getRoute().size();
    output<<"\n";
    return (output);
}

bool operator==(Tour& t1,Tour& t2){
    vector<int> routeT1= t1.getRoute();
    vector<int> routeT2= t2.getRoute();
    for (unsigned i=0; i<routeT1.size();i++){
        if(routeT1[i]!=routeT2[i]){
            return false;
        }
    }
    return true;
}
bool operator!=(Tour& t1, Tour& t2){
    return !(t1==t2);
}

