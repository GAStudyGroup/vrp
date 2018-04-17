#include <algorithm>
#include "Trim.hpp"
#include "TourRepairer.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

Tour Trim::applyTrim(Tour& tour){
    if(willApply(tour)){
        std::cout<<"Aplicando"<<std::endl;
        auto subtours= TourRepairer::splitSubTours(tour.getRoute());
        getToursToTrim(subtours,getExtraSubsN(tour));
        TourRepairer::printSubtours(subtours);
        return tour;
    }else{
        std::cout<<"Não Aplicando"<<std::endl;
        return tour;
    }    
}
 vector<int> Trim::getToursToTrim(vector<vector<int>> subtours, int extraN){
    vector<int> ids;
    vector<std::pair<int,vector<int>>> ranking;
    int id=0;
    for(auto sub: subtours){
        ranking.push_back(make_pair(id,sub));        
        id++;
    }
    std::sort(ranking.begin(),ranking.end(),[](auto &a, auto &b){
        return TourUtils::getSubCharge(a.second) < TourUtils::getSubCharge(b.second);
    });
    for (auto val: ranking){
        if(TourUtils::getSubCharge(val.second)>0){
            extraN--;
            ids.push_back(val.first);
        }
        if(extraN==0){
            break;
        }
    }
    for(auto i: ids){
        std::cout<< "Tour: "<<i<<std::endl;
    }
    return ids;
 }

bool Trim::willApply(Tour& tour){
    if(tour.explodeSubTours().size()<=Globals::customerMap.getMnv()){
        return false;
    }
    auto split= TourRepairer::splitSubTours(tour.getRoute());
    if(TourRepairer::getOverloadedSubs(split).size()>0){
        return false;
    }
    return true;
}
int Trim::getExtraSubsN(Tour& tour){
    int n=0;
    n= tour.explodeSubTours().size() - Globals::customerMap.getMnv();
    return n;
}


