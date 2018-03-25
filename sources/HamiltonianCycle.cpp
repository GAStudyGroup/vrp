#include <algorithm>

#include "HamiltonianCycle.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

HamiltonianCycle::correlationMap ranking;
string depotCopyToken{"'"};

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::toHamiltonianCycle(Tour red, Tour blue){
    vector<vector<int>> redSubs{explodeSubTours(red.getRoute(), Configs::customerMap.getDepotId())};
    vector<vector<int>> blueSubs{explodeSubTours(blue.getRoute(), Configs::customerMap.getDepotId())};

    choosenSubs preChoose = mapDepots(redSubs, blueSubs);
    cout << "PreChoose"<<endl;
    for(auto it : preChoose){
        cout << "Red " << it.first << " Blue " << it.second << endl;
    }

    parentsHamiltonian newTours = rebuildTours(redSubs, blueSubs, preChoose);

    newTours.first = createDepotCopies(newTours.first);
    newTours.second = createDepotCopies(newTours.second);

    cout << "NewRedTour :";
    for(auto it : newTours.first){
        cout << it << " ";
    }
    cout << endl;
    cout << "NewBlueTour :";
    for(auto it : newTours.second){
        cout << it << " ";
    }
    cout << endl;

    return(newTours);
}

// Will generate a list of pre selected combinations of subTours
HamiltonianCycle::choosenSubs HamiltonianCycle::mapDepots(subTours& redSubs, subTours& blueSubs){
    unsigned redSubsSize{redSubs.size()}, blueSubsSize{blueSubs.size()};

    choosenSubs preChoose;

    cout << "Size " << blueSubsSize << endl;
    for(unsigned r=0; r<redSubsSize; r++){
        for(unsigned b=0; b<blueSubsSize; b++){
            ranking[r].push_back(Correlation(0, 0));
            unsigned redInsideSize{redSubs[r].size()}; 
            cout << "SubBlue: ";
            for(int c : blueSubs[b]){
                cout << c <<" ";
            }
            cout << endl;
            for(unsigned rInside=0; rInside<redInsideSize; rInside++){
                cout << "Procurar " << redSubs[r][rInside]<<endl;
                // return the position of element in vector, if return == size, the element not found in vector
                auto position = std::distance(blueSubs[b].begin(), std::find(blueSubs[b].begin(), blueSubs[b].end(), redSubs[r][rInside]));
                if(!(position==redInsideSize)){
                    if(position==0 || position==(redInsideSize-1)){
                        ranking[r][b].score+=5;
                        ranking[r][b].numberInOut++;
                    }
                    ranking[r][b].score+=2;
               }
            }
            cout << "r " << r << " b " << b << " " << ranking[r].size()<<endl;
            cout << "SCORE " << ranking[r][b].score << endl;
            cout << "N_InOut " << ranking[r][b].numberInOut << endl;
        }
        int countB{0}, maxScore{0}; 
        preChoose.push_back(std::make_pair(-1, -1));
        for(auto it : ranking[r]){
            if(maxScore < it.score){
                maxScore = it.score;
                preChoose[r].first=r;
                preChoose[r].second=countB;
            }
            countB++;
        }
    }
    return(preChoose);
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::rebuildTours(subTours& redTours, subTours& blueTours, choosenSubs& choosen) {
    int depotId{Configs::customerMap.getDepotId()};
    parentsHamiltonian tours;

    cout << "RedSubs"<<endl;
    for(auto sub : redTours){
        for(auto c : sub) {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << "BlueSubs"<<endl;
    for(auto sub : blueTours){
        for(auto c : sub) {
            cout << c << " ";
        }
        cout << endl;
    }

    cout << "Choosen"<<endl;
    for(auto it : choosen){
        cout << it.first << " " << it.second << endl;
        tours.first.push_back(std::to_string(depotId));
        tours.second.push_back(std::to_string(depotId));
        for(int c : redTours[it.first]){
            tours.first.push_back(std::to_string(c));
        }
        for(int c : blueTours[it.second]){
            tours.second.push_back(std::to_string(c));
        }
    }
    return(tours);  
}

vector<string> HamiltonianCycle::createDepotCopies(vector<string> tour){
    vector<string> copiesTour;
    string token{depotCopyToken};
    const string depotId{std::to_string(Configs::customerMap.getDepotId())};

    for(string customer : tour){
        if(!customer.compare(depotId)){
            copiesTour.push_back(customer+token);
            token += depotCopyToken;
        } else {
            copiesTour.push_back(customer);
        }
    }
    return(copiesTour);
}