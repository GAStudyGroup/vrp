#include<iostream>
#include<string>
#include<algorithm>

#include "TourRepairer.hpp"
#include "Configs.hpp"
#include "CustomerMap.hpp"
#include "TourUtils.hpp"

using std::cout;
using std::endl;

Tour TourRepairer::repairTour(Tour& original){
    //cout<<"Iniciando Reparação"<<endl;
    auto subtours= splitSubTours(original.getRoute());
    sortSubsByCharge(subtours);
    //printSubtours(subtours);
    int i=0;
    while(i<1){
        i++;
        changeCustomer(subtours);
    }
    
    original=tourRebuilder(subtours);
    return original;
}

int TourRepairer::getTourCharge(vector<int>& subtour){
    int charge=0;
    for(auto customer: subtour){
        charge+=Configs::customerMap.getCustomer(std::to_string(customer)).getDemand();
    }
    return charge;
}

void TourRepairer::changeCustomer(vector<vector<int>>& subtours){//Verificar mais tarde
    vector<int> overIds=getOverloadedSubs(subtours);
    int it=0;
    for(auto id:overIds){
        //cout<<"-----------Entrando: -----------"<<id<<endl;
        // printSubtours(subtours);
        int idHeavier=getHeaviestCustomer(subtours[id]);
        // cout<<"------------Passou get-------------"<<endl;
        eraseElement(subtours[id],idHeavier);
        // cout<<"-----------Erase -----------"<<endl;
        // printSubtours(subtours);
        // cout<<"-----------Push-----------"<<endl;
        subtours[it].push_back(idHeavier);
        // printSubtours(subtours);
        // cout<<"-----------Sort-----------"<<endl;
        sortSubsByCharge(subtours);
        //it++;
        
    }
}
vector<vector<int>> TourRepairer::splitSubTours(vector<int>& tour){
    vector<vector<int>> subtours;
    vector<int> aux;
    int it=-1;
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
    for(unsigned i=0;i<tour.size();i++){
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
        cout<<" Charge: "<<getTourCharge(subtour);
        cout<<" Heaviest Customer: "<<getHeaviestCustomer(subtour);
        it++;
        cout<<endl;
    }
}

vector<int> TourRepairer::getOverloadedSubs(vector<vector<int>>& subtours){
    vector<int> SubsIds;
    int id=-1;;
    for(auto subtour: subtours){
        id++;
        if(getSubCharge(subtour)>Configs::customerMap.getTruckCapacity()){
            SubsIds.push_back(id);
        }
    }
    return SubsIds;
}

void TourRepairer::sortSubsByCharge(vector<vector<int>>& subtours){
    std::sort(subtours.begin(),subtours.end(),
     [](vector<int>& a, vector<int>& b) {        
        return  getSubCharge(a) < getSubCharge(b);
    });
    
}

int TourRepairer::getHeaviestCustomer(vector<int>& subtour){
    int customerId= *std::max_element(subtour.begin(),subtour.end(),[](int& a, int&b){
        return (Configs::customerMap.getCustomer(a).getDemand()
        <
        Configs::customerMap.getCustomer(b).getDemand());
    });
    return customerId;
}

void TourRepairer::eraseElement(vector<int>& subtour,int element){
    subtour.erase(find(subtour.begin(),subtour.end(),element));
}