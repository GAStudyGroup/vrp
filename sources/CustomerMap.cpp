#include <algorithm>
#include "CustomerMap.hpp"
CustomerMap::CustomerMap(){}

CustomerMap::CustomerMap(std::vector<Customer> list, int capacity, int trucks):map(list), truckCapacity(capacity), nTrucks(trucks){}

Customer CustomerMap::getCustomer(const int id){
    auto it = std::find_if(map.begin(), map.end(), [&id](const Customer& obj) { return (obj.getId() == id); });
    if (it != map.end()) {
        return (*it);
    }else{
        // warning...
        return (Customer());
    }
}

Customer CustomerMap::getCustomer(const string id){
    auto it = std::find_if(map.begin(), map.end(), [&id](const Customer& obj) { return (obj.getId() == stoi(id)); });
    if (it != map.end()) {
        return (*it);
    }else{
        // warning...
        return (Customer());
    }
}

std::vector<Customer>& CustomerMap::getMap(){
    return(map);
}

int CustomerMap::getDepotId(){
    auto it = std::find_if(map.begin(), map.end(), [](const Customer& obj) { return (obj.getDemand() == 0); });
    if (it != map.end()) {
        return (*it).getId();
    }else{
        return(0);
    }
}

void CustomerMap::setMap(std::vector<Customer> map){
    this->map = map;
}

void CustomerMap::setNumberTrucks(int trucks){
    nTrucks = trucks;
}
void CustomerMap::setTruckCapacity(double capacity){
    truckCapacity = capacity;
}

int CustomerMap::getNumberTrucks(){
    return (nTrucks);
}
double CustomerMap::getTruckCapacity(){
    return (truckCapacity);
}