#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include <string>

// return the euclidian distance beetween two points in map
double distance(const int, const int);

double distance(const std::string, const std::string);

double distance(const double xA, const double yA, const double xB, const double yB);
#endif