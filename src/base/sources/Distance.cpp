#include "Distance.hpp"
#include <cmath>
#include "Configs.hpp"

double distance(const int a, const int b){
    return std::round((std::sqrt(std::pow(Globals::customerMap.getCustomer(a).getX() - Globals::customerMap.getCustomer(b).getX(), 2) + pow(Globals::customerMap.getCustomer(a).getY() - Globals::customerMap.getCustomer(b).getY(), 2))));
}

double distance(const std::string a, const std::string b) {
    return(distance(std::stoi(a), std::stoi(b)));
}
