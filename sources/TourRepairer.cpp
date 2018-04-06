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
    //sortSubsByCharge(subtours);
    //printSubtours(subtours);
    changeCustomers(subtours);    
    original=tourRebuilder(subtours);
    return original;
}

int TourRepairer::getTourCharge(vector<int>& subtour){
    int charge=0;
    for(auto customer: subtour){
        charge+=getCustomerDemand(customer);
    }
    return charge;
}

void TourRepairer::changeCustomers(vector<vector<int>>& subtours){//Verificar mais tarde
    changeHeaviestCustomers(subtours);
    changeLightestCustomers(subtours);
}
 void TourRepairer::changeHeaviestCustomers(vector<vector<int>>& subtours){
    vector<int> overIds=getOverloadedSubs(subtours);
        for(int id: overIds){//Ids dos sobrecaregados
            for(unsigned i=0;i<subtours.size();i++){ //Itera por todos os outros subs
                if(i!=(unsigned)id){ //Não altera em relação a ele mesmo
                    for(unsigned j=0;j<subtours[id].size();j++){ //Testa todas possibilidades 1 mais pesado, 2 etc..
                        int idHeaviest=getHeaviestCustomer(subtours[id],j);
                        if(!willOverload(subtours[i],idHeaviest) && (idHeaviest!=
                            Configs::customerMap.getDepotId())){
                                eraseElement(subtours[id],idHeaviest);
                                subtours[i].push_back(idHeaviest);//Adicionar aqui possível heuristica de dist
                        }
                    }
                }
            }        
        }
 }
 void TourRepairer::changeLightestCustomers(vector<vector<int>>& subtours){
    vector<int> overIds=getOverloadedSubs(subtours);
    for(int id: overIds){
        for(unsigned i=0;i<subtours.size();i++){
            if(i!=(unsigned)id){
                int idLightest = getLightestCustomer(subtours[id]);
                if(!willOverload(subtours[i],idLightest)&&(idLightest!=
                Configs::customerMap.getDepotId())){
                    eraseElement(subtours[id],idLightest);
                    subtours[i].push_back(idLightest);
                }
            }
        }        
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
        cout<<" Heaviest Customer: "<<getHeaviestCustomer(subtour,0);
        it++;
        cout<<endl;
    }
}

vector<int> TourRepairer::getOverloadedSubs(vector<vector<int>>& subtours){
    vector<int> SubsIds;
    int id=-1;;
    for(auto subtour: subtours){
        id++;
        if(getTourCharge(subtour)>Configs::customerMap.getTruckCapacity()){
            SubsIds.push_back(id);
        }
    }
    return SubsIds;
}

void TourRepairer::sortSubsByCharge(vector<vector<int>>& subtours){
    std::sort(subtours.begin(),subtours.end(),
     [this](vector<int>& a, vector<int>& b) {        
        return  getTourCharge(a) < getTourCharge(b);
    });
    
}

int TourRepairer::getHeaviestCustomer(vector<int> subtour,int id){
    std::sort(subtour.begin(),subtour.end(),[this](int &a, int &b){
        return getCustomerDemand(a) > getCustomerDemand(b);
    });
    int customerId=subtour[id];
    return customerId;
}
int TourRepairer::getLightestCustomer(vector<int> subtour){
    std::sort(subtour.begin(),subtour.end(),[this](int &a, int &b){
        return getCustomerDemand(a) < getCustomerDemand(b);
    });
    int customerId=subtour[0]; //Sempre pega o menor
    return customerId;
}

void TourRepairer::eraseElement(vector<int>& subtour,int element){
    subtour.erase(find(subtour.begin(),subtour.end(),element));
}

int TourRepairer::getCustomerDemand(int id){
    return Configs::customerMap.getCustomer(id).getDemand();
}

bool TourRepairer::willOverload(vector<int>& subtour, int id){
    int totalCharge= (getTourCharge(subtour) + getCustomerDemand(id));
    if(totalCharge>Configs::customerMap.getTruckCapacity()){
        // cout<<"Over"<<endl;
        return true;
    }    
    return false;
}