#include "Tour.hpp"
#include "Utils.hpp"
#include "Configs.hpp"
#include "Fitness.hpp"
#include <iostream>

Tour::Tour() {}

Tour::Tour(vector<int> route)
    : route(route)
{
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

    for (int c : t.getRoute()) {
        output << c << "\n";
    }
    output << "Fitness: " << t.getFitness() << "\n";
    return (output);
}