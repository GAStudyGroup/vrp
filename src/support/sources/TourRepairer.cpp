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
    changeCustomers(subtours);    
    original=tourRebuilder(subtours);
    return original;
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
                            Globals::customerMap.getDepotId())){
                                eraseElement(subtours[id],idHeaviest);
                                //Adicionar aqui possível heuristica de dist
                                //subtours[i].push_back(idHeaviest);
                                subtours[i].emplace(subtours[i].begin()+
                                checkBestPosition(subtours[i],idHeaviest),idHeaviest);
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
                Globals::customerMap.getDepotId())){
                    eraseElement(subtours[id],idLightest);
                    //subtours[i].push_back(idLightest);
                    subtours[i].emplace(subtours[i].begin()+
                    checkBestPosition(subtours[i],idLightest),idLightest);
                }
            }
        }        
    }
 }
vector<vector<int>> TourRepairer::splitSubTours(vector<int>& tour){
    vector<vector<int>> subtours= Tour(tour).explodeSubTours();
    for(unsigned i=0;i<subtours.size();i++){
        subtours[i].emplace(subtours[i].begin(),Globals::customerMap.getDepotId());
    }
    int emptyRoutes=Configs::truckNumber-subtours.size();
    vector<int> aux;
    aux.push_back(Globals::customerMap.getDepotId());
    if(emptyRoutes>0){
        for(int i=0;i<emptyRoutes;i++){
            subtours.push_back(aux);
        }
    }
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
        if(tour[i]==Globals::customerMap.getDepotId()){
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
        cout<<" Charge: "<<TourUtils::getSubCharge(subtour);
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
        if(TourUtils::getSubCharge(subtour)>Globals::customerMap.getTruckCapacity()){
            SubsIds.push_back(id);
        }
    }
    return SubsIds;
}

void TourRepairer::sortSubsByCharge(vector<vector<int>>& subtours){
    std::sort(subtours.begin(),subtours.end(),
     [](vector<int>& a, vector<int>& b) {        
        return  TourUtils::getSubCharge(a) < TourUtils::getSubCharge(b);
    });
    
}

int TourRepairer::getHeaviestCustomer(vector<int> subtour,int id){
    std::sort(subtour.begin(),subtour.end(),[](int &a, int &b){
        return getCustomerDemand(a) > getCustomerDemand(b);
    });
    int customerId=subtour[id];
    return customerId;
}
int TourRepairer::getLightestCustomer(vector<int> subtour){
    std::sort(subtour.begin(),subtour.end(),[](int &a, int &b){
        return getCustomerDemand(a) < getCustomerDemand(b);
    });
    int customerId=subtour[0]; //Sempre pega o menor
    return customerId;
}

void TourRepairer::eraseElement(vector<int>& subtour,int element){
    subtour.erase(find(subtour.begin(),subtour.end(),element));
}

int TourRepairer::getCustomerDemand(int id){
    return Globals::customerMap.getCustomer(id).getDemand();
}

bool TourRepairer::willOverload(vector<int>& subtour, int id){
    int totalCharge= (TourUtils::getSubCharge(subtour) + getCustomerDemand(id));
    if(totalCharge>Globals::customerMap.getTruckCapacity()){
        // cout<<"Over"<<endl;
        return true;
    }    
    return false;
}

int TourRepairer::checkBestPosition(vector<int> subtour,int customer){
    if(subtour.size()==1){
        return 1;
    }
    vector<std::pair<int,vector<int>>> lista;
    for(unsigned i=1;i<subtour.size();i++){
        vector<int> temp = subtour;
        temp.emplace(temp.begin()+i,customer);
        lista.push_back(std::make_pair(i,temp));
    }
    std::sort(lista.begin(),lista.end(),[](auto &a, auto &b){
        return TourUtils::getSubDistance(a.second) < TourUtils::getSubDistance(b.second);
    });
    return lista[0].first;
}