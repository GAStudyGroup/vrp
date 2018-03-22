#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <algorithm>

#include "Customer.hpp"

class Map {

    public:
        Map();
        Map(std::vector<Customer>, int, int=8);

        Customer getCustomer(const int);
        std::vector<Customer>& getMap();
        int getDepotId();

        void setMap(std::vector<Customer>);
        void setNumberTrucks(int);
        void setTruckCapacity(double);

        int getNumberTrucks();
        double getTruckCapacity();
    private:
        std::vector<Customer> map;
        double truckCapacity;
        int nTrucks;
};

#endif