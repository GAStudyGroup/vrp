#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

#include "Customer.hpp"

using std::vector;

class Population {

    using Tour = vector<int>;

public:
    Population();
    Population(vector<Tour>&);

    void addNewTour(Tour&);

    vector<Tour>& getPop();
private:
    vector<Tour> pop;

    friend std::ostream& operator<<(std::ostream&, Population&);
};

#endif