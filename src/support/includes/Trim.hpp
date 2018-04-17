#ifndef TRIM_HPP
#define TRIM_HPP

#include "Tour.hpp"
namespace Trim{
    Tour applyTrim(Tour&);
    bool willApply(Tour& );    
    vector<int> getToursToTrim(vector<vector<int>>,int);
    int getExtraSubsN(Tour&);
    vector<vector<int>> moveCustomers(vector<vector<int>>,int);
}

#endif