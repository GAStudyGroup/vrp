#include "Tour.hpp"

#include <iostream>

Tour::Tour() {}

Tour::Tour(vector<Customer> route)
    : route(route)
{
}

void Tour::setRoute(vector<Customer> route)
{
    this->route = route;
}

vector<Customer>& Tour::getRoute()
{
    return route;
}

int Tour::getFitness()
{ // Irá retornar a fitness do Tour específico
// AINDA PRECISA DA FITNESS
}

ostream& operator<<(ostream& output, Tour& t)
{ // Overload de operador para impressão da população

    for (Customer c : t.getRoute()) {
        output << c << "\n";
    }
    output << "Fitness: " << t.getFitness() << "\n";
    return (output);
}