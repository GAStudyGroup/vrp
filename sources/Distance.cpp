#include "Distance.hpp"
#include <cmath>
#include "Configs.hpp"
double distance(const int a, const int b){
    //return(std::round(std::sqrt(std::pow(Configs::customerMap.getCustomer(a).getX() - Configs::customerMap.getCustomer(b).getX(), 2) + pow(Configs::customerMap.getCustomer(a).getY() - Configs::customerMap.getCustomer(b).getY(), 2))));
    return std::round((std::sqrt(std::pow(Configs::customerMap.getCustomer(a).getX() - Configs::customerMap.getCustomer(b).getX(), 2) + pow(Configs::customerMap.getCustomer(a).getY() - Configs::customerMap.getCustomer(b).getY(), 2))));
}
