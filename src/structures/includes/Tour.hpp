#ifndef TOUR_HPP
#define TOUR_HPP

#include <vector>
#include <string>
#include <iostream>
#include <deque>
#include "Customer.hpp"

using std::vector;
class Tour {

    friend std::ostream& operator<<(std::ostream&, Tour&);
    friend bool operator==(Tour&,Tour&);
    friend bool operator!=(Tour&,Tour&);

public:
    Tour(); 
    Tour(vector<Customer>);
    Tour(vector<int>);
    Tour(vector<std::string>);
    Tour(std::deque<int>);
    void setRoute(vector<int>);
    vector<int>& getRoute();
    double getFitness();
    double getDist();
    vector<int> getAllCharges();
    vector<vector<int>> explodeSubTours();
    bool isValid();
    int subToursUsed();
private:
    vector<int> route;
};

#endif