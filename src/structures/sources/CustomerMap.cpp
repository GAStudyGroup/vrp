#include <algorithm>
#include <cmath>
#include "CustomerMap.hpp"
CustomerMap::CustomerMap(){}

CustomerMap::CustomerMap(std::vector<Customer> list, double cap){
    map=list;
    maxTruckCapacity=cap;
    mnv=calcMnv();
}
int CustomerMap::getMnv(){
    return mnv;
}

Customer CustomerMap::getCustomer(const int id){
    auto it = std::find_if(map.begin(), map.end(), [&id](const Customer& obj) { return (obj.getId() == id); });
    if (it != map.end()) {
        return (*it);
    }else{
        // warning...
        return (Customer());
    }
}

Customer CustomerMap::getCustomer(const std::string id){
    int cId{std::stoi(id)};
    auto it = std::find_if(map.begin(), map.end(), [&cId](const Customer& obj) { return (obj.getId() == cId); });
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

double CustomerMap::getTruckCapacity() {
    return(maxTruckCapacity);
}

int CustomerMap::calcMnv(){
    double demandSum=0;
    for (auto customer: this->getMap()){
        demandSum+=customer.getDemand();
    }  
    int mnv=std::ceil(demandSum/this->getTruckCapacity());
    return (mnv);
}