#include "Map.hpp"
Map::Map(){}

Map::Map(std::vector<Customer> list, int capacity, int trucks):map(list), truckCapacity(capacity), nTrucks(trucks){}

Customer Map::getCustomer(const int id){
    auto it = std::find_if(map.begin(), map.end(), [&id](const Customer& obj) { return (obj.getId() == id); });
    if (it != map.end()) {
        return (*it);
    }else{
        // warning...
        return (Customer());
    }
}

std::vector<Customer>& Map::getMap(){
    return(map);
}

int Map::getDepotId(){
    auto it = std::find_if(map.begin(), map.end(), [](const Customer& obj) { return (obj.getDemand() == 0); });
    if (it != map.end()) {
        return (*it).getId();
    }else{
        return(0);
    }
}

void Map::setMap(std::vector<Customer> map){
    this->map = map;
}

void Map::setNumberTrucks(int trucks){
    nTrucks = trucks;
}
void Map::setTruckCapacity(double capacity){
    truckCapacity = capacity;
}

int Map::getNumberTrucks(){
    return (nTrucks);
}
double Map::getTruckCapacity(){
    return (truckCapacity);
}