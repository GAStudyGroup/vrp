#ifndef POPULATION_HPP
#define POPULATION_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <limits>

#include "Tour.hpp"
#include "Customer.hpp"

using std::vector;

class Population {

public:
    Population();
    Population(vector<Tour>&);

    void addNewTour(Tour&);

    vector<Tour>& getPop();
    void sortPop();

    Tour getBestSolution();
    int totalToursValid();
private:
    vector<Tour> pop;

    friend std::ostream& operator<<(std::ostream&, Population&);
};

#endif