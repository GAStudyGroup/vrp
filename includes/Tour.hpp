#ifndef TOUR_HPP
#define TOUR_HPP

#include <cmath>
#include <vector>

#include "Customer.hpp"
#include "Utils.hpp"

using std::ostream;
using std::round;
using std::vector;

class Tour {

    friend ostream& operator<<(ostream&, Tour&);

public:
    Tour(); 
    Tour(vector<Customer>);

    void setRoute(vector<Customer>);

    vector<Customer>& getRoute();

    int getFitness();
private:
    vector<Customer> route;
};

#endif