#ifndef CUSTOMERMAP_HPP
#define CUSTOMERMAP_HPP

#include <vector>
#include <string>

#include "Customer.hpp"

class CustomerMap {

    public:
        CustomerMap();
        CustomerMap(std::vector<Customer>, double);

        Customer getCustomer(const int);
        Customer getCustomer(const std::string);
        std::vector<Customer>& getMap();
        int getDepotId();
        void setMap(std::vector<Customer>);

        double getTruckCapacity();
    private:
        std::vector<Customer> map;
        double maxTruckCapacity;
};

#endif