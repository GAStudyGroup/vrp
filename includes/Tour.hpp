#ifndef TOUR_HPP
#define TOUR_HPP

#include <cmath>
#include <vector>
#include <string>

#include "Customer.hpp"

using std::ostream;
using std::round;
using std::vector;
using std::string;

class Tour {

    friend ostream& operator<<(ostream&, Tour&);
    friend bool operator==(Tour&,Tour&);
    friend bool operator!=(Tour&,Tour&);

public:
    Tour(); 
    Tour(vector<Customer>);
    Tour(vector<int>);
    Tour(vector<string>);

    void setRoute(vector<int>);

    vector<int>& getRoute();

    double getFitness();
    double getDist();
private:
    vector<int> route;
};

#endif