#include<iostream>

#include "TourRepairer.hpp"
#include "Configs.hpp"
using std::cout;
using std::endl;

Tour TourRepairer::repairTour(Tour& original){
    cout<<"Iniciando Reparação"<<endl;
    auto subtours= splitSubTours(original.getRoute());
    printSubtours(subtours);
    return original;
}

vector<vector<int>> TourRepairer::splitSubTours(vector<int>& tour){
    vector<vector<int>> subtours;
    vector<int> aux;
    int it=0;
    for(auto customer:tour){
        aux.push_back(customer);
        if(customer==Configs::customerMap.getDepotId() && it==0){
            it++;
            continue;
        }
        else if(customer==Configs::customerMap.getDepotId()){
            subtours.push_back(aux);
            aux.clear();                
        }
        it++;
    }
    return subtours;
}

void TourRepairer::printSubtours(vector<vector<int>>& subtours){
    int it=1;
    for(auto subtour: subtours){
        cout<<"SubTour "<<it<<": ";
        for(auto customer: subtour){
            cout<<customer<<" ";
        }
        it++;
        cout<<endl;
    }
}