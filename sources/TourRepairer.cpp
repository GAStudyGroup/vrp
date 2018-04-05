#include<iostream>

#include "TourRepairer.hpp"
#include "Configs.hpp"
using std::cout;
using std::endl;

Tour TourRepairer::repairTour(Tour& original){
    cout<<"Iniciando Reparação"<<endl;
    auto subtours= splitSubTours(original.getRoute());
    printSubtours(subtours);
    original=tourRebuilder(subtours);
    return original;
}

vector<vector<int>> TourRepairer::splitSubTours(vector<int>& tour){
    vector<vector<int>> subtours;
    vector<int> aux;
    int it=-1;
    bool alreadyDep=false;
    //int lastDepotPos=findLastDepotPosition(tour);
    for(auto customer:tour){
        it++;
        if(Configs::customerMap.getDepotId()==customer){
            if(it!=0){
                subtours.push_back(aux);
                aux.clear();
                aux.push_back(customer);
            }else{
                aux.push_back(customer);    
            }
        }else{
            aux.push_back(customer);
        }
    }
    subtours.push_back(aux);
    return subtours;
}
vector<int> TourRepairer::tourRebuilder(vector<vector<int>>& subtours){
    vector<int> newTour;
    for(auto subtour:subtours){
        for(auto customer: subtour){
            newTour.push_back(customer);
        }
    }
    return newTour;
}
int TourRepairer::findLastDepotPosition(vector<int>&tour){
    int lastDepotPos=-1;
    for(int i=0;i<tour.size();i++){
        if(tour[i]==Configs::customerMap.getDepotId()){
            lastDepotPos=i;
        }
    }
    return lastDepotPos;
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