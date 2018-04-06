#ifndef TOURREPAIRER_HPP
#define TOURREPAIRER_HPP

#include "Tour.hpp"
using std::vector;
class TourRepairer{
    //Step 1- Find the overloaded tours
    //Step 2- Find the customer whom demands the most
    //Step 3- Move this customer to the tour with less load
    //Step 4- Test inside the tour to check which position will impact less on the overral distance
    // Repeat until there is not any overloaded tours

    //Colocar para não alterar caso for estourar
    //Encontrar um modo em que todos os subtour sejam válidos
    public:
    Tour repairTour(Tour&);
    
    private:
    vector<vector<int>> splitSubTours(vector<int>&);
    void printSubtours(vector<vector<int>>&);
    int findLastDepotPosition(vector<int>&tour);
    vector<int> tourRebuilder(vector<vector<int>>&tour);
    int getTourCharge(vector<int>&);
    vector<int> getOverloadedSubs(vector<vector<int>>&);
    int getHeaviestCustomer(vector<int>,int id);
    int getLightestCustomer(vector<int>);
    void sortSubsByCharge(vector<vector<int>>&);
    void changeCustomers(vector<vector<int>>&);
    void eraseElement(vector<int>&,int);
    bool willOverload(vector<int>&, int);
    int getCustomerDemand(int);
    void changeHeaviestCustomers(vector<vector<int>>&);
    void changeLightestCustomers(vector<vector<int>>&);
};
#endif 