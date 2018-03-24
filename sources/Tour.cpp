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
    // output<<"Tour: \n";
    for (int c : t.getRoute()) {
        output << c << " ";
    }
    output << "\nFitness: " << t.getFitness();
    output << "\nDistance: "<<t.getDist();
    /* output << "\nCharges:";
    for(auto charge:getAllCharges(t.getRoute())){
        output<<" "<<charge;
    } */
    output<<"\n";
    return (output);
}