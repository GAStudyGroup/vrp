#include <algorithm>

#include "HamiltonianCycle.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

// RETIRAR DEPOIS
bool validaMerdaChoosen(vector<pair<int,int>> merda){

    vector<int> reds;
    vector<int> blues;

    for(auto it : merda){
        reds.push_back(it.first);
        blues.push_back(it.second);
    }
    std::sort(reds.begin(), reds.end());
    std::sort(blues.begin(), blues.end());

    bool redB{std::adjacent_find(reds.begin(), reds.end())==reds.end()};
    bool blueB{std::adjacent_find(blues.begin(), blues.end())==blues.end()};

    return(redB && blueB);
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::toHamiltonianCycle(Tour red, Tour blue){
    HamiltonianCycle obj;
    obj.correlationDuplicate = (red.getRoute().size()*0.2);
    obj.correlationIn_Out = obj.correlationDuplicate*3; 

    obj.redSubs = explodeSubTours(red.getRoute(), Configs::customerMap.getDepotId());
    obj.blueSubs = explodeSubTours(blue.getRoute(), Configs::customerMap.getDepotId());

    obj.generateRanking();  

    obj.choosenToursToMap();

    std::sort(obj.choosen.begin(), obj.choosen.end(), [](auto &left, auto &right){ return(left.first < right.first); });

/*     cout << "FINAL CHOOSEN" << endl;
    for(auto it : obj.choosen){
        cout << it.first << " " << it.second << endl;
    } */

    if(!validaMerdaChoosen(obj.choosen)){
        cout << "DEU MERDA NA CHOOSEN"<<endl;
        exit(0);
    }

    parentsHamiltonian newTours = obj.rebuildTours();

    newTours.first = obj.createDepotCopies(newTours.first);
    newTours.second = obj.createDepotCopies(newTours.second);

    return(newTours);
}

void HamiltonianCycle::generateRanking(){
    unsigned redSubsSize{redSubs.size()}, blueSubsSize{blueSubs.size()};
    choosenSubs choosen;
    for(unsigned red=0; red<redSubsSize; red++){
        for(unsigned blue=0; blue<blueSubsSize; blue++){

            ranking[red].push_back(Correlation(0, 0));
            unsigned redInsideSize{redSubs[red].size()};
            unsigned blueInsideSize{blueSubs[blue].size()};
            /* cout << "SubRed: ";
            for(int c : redSubs[red]){
                cout << c <<" ";
            }cout << endl;
            cout << "SubBlue: ";
            for(int c : blueSubs[blue]){
                cout << c <<" ";
            }cout << endl; */

            for(unsigned redElement=0; redElement<redInsideSize; redElement++){
                //cout << "Procurar " << redSubs[red][redElement] << " ";

                auto position = std::distance(blueSubs[blue].begin(), find(blueSubs[blue].begin(), blueSubs[blue].end(), redSubs[red][redElement]));

                if(!(position>=blueInsideSize)){
                    //cout << "achou ";
                    if((position==0 || position==(redInsideSize-1))&&(redElement==0 || redElement==(redInsideSize-1))){
                        //cout << " InOut ";
                        ranking[red][blue].score+=correlationIn_Out;
                        ranking[red][blue].numberInOut++;
                    }
                    ranking[red][blue].score+=correlationDuplicate;
               }
               //cout << endl;
            }
        }
    }
}

void HamiltonianCycle::choosenToursToMap(){
    vector<int> redNotChoosen, blueNotChoosen;

    for(unsigned i=0; i<redSubs.size(); i++){
        redNotChoosen.push_back(i);
    }
    for(unsigned i=0; i<blueSubs.size(); i++){
        blueNotChoosen.push_back(i);
    }

    for(int red : redNotChoosen){
        for(int blue : blueNotChoosen){
            resolveScore.push_back(ResolveScore(red, blue, ranking[red][blue].score));
        }
    }

    std::sort(resolveScore.begin(), resolveScore.end(), [](auto &left, auto &right){ return(left.score > right.score); });

    for(auto it : resolveScore){
        vector<int>::iterator findRedNotChoosen{find(redNotChoosen.begin(), redNotChoosen.end(), it.subRed)};
        vector<int>::iterator findBlueNotChoosen{find(blueNotChoosen.begin(), blueNotChoosen.end(), it.subBlue)};

        bool inRedNotChoosen{findRedNotChoosen != redNotChoosen.end()};
        bool inBlueNotChoosen{findBlueNotChoosen != blueNotChoosen.end()};

        if(inRedNotChoosen && inBlueNotChoosen){
            choosen.push_back(std::make_pair(it.subRed, it.subBlue));

            blueNotChoosen.erase(findBlueNotChoosen);
            redNotChoosen.erase(findRedNotChoosen);
        }
    }
}

HamiltonianCycle::parentsHamiltonian HamiltonianCycle::rebuildTours(){
    int depotId{Configs::customerMap.getDepotId()};
    parentsHamiltonian tours;
    vector<int> alreadyRebuildRed, alreadyRebuildBlue;
    unsigned limitRed{redSubs.size()};
    unsigned limitBlue{blueSubs.size()};
    unsigned choosenSize{choosen.size()};

    for(unsigned i=0; i<choosenSize; i++){
        if(i<limitRed){
            tours.first.push_back(std::to_string(depotId));
        }
        if(i<limitBlue){
            tours.second.push_back(std::to_string(depotId));
        }

        alreadyRebuildRed.push_back(choosen[i].first);
        for(int redC : redSubs[choosen[i].first]){
            tours.first.push_back(std::to_string(redC));
        }

        alreadyRebuildBlue.push_back(choosen[i].second);
        for(int blueC : blueSubs[choosen[i].second]){
            tours.second.push_back(std::to_string(blueC));
        }
    }
    std::sort(alreadyRebuildRed.begin(), alreadyRebuildRed.end(), [](int left, int right) { return(left>right); });
    std::sort(alreadyRebuildBlue.begin(), alreadyRebuildBlue.end(), [](int left, int right) { return(left>right); });

    for(unsigned i=0; i<alreadyRebuildRed.size(); i++){
        redSubs.erase(redSubs.begin()+alreadyRebuildRed[i]);
        blueSubs.erase(blueSubs.begin()+alreadyRebuildBlue[i]);
    }

    if(!redSubs.empty()){
        /* vector<string>::iterator it = find(tours.second.begin(), tours.second.end(), std::to_string(depotId));
        tours.second.emplace(it, std::to_string(depotId)); */
        tours.second.push_back(std::to_string(depotId));
        //tours.second.emplace(tours.second.begin(), std::to_string(depotId));
        for(vector<int> red : redSubs){
            tours.first.push_back(std::to_string(depotId));
            for(int redC : red){
                tours.first.push_back(std::to_string(redC));
            }
        }
    }
    if(!blueSubs.empty()){
        //vector<string>::iterator it = find(tours.second.begin(), tours.second.end(), std::to_string(depotId));
        //tours.first.emplace(it, std::to_string(depotId));
        tours.first.push_back(std::to_string(depotId));
        //tours.first.emplace(tours.first.begin(), std::to_string(depotId));
        for(vector<int> blue : blueSubs){
            tours.second.push_back(std::to_string(depotId));
            for(int blueC : blue){
                tours.second.push_back(std::to_string(blueC));
            }
        }
    }

    /* if(tours.first.size() != tours.second.size()){
        if(tours.first.size() > tours.second.size()){
            tours.second.emplace(tours.second.begin(), std::to_string(depotId));
        }else{
            tours.first.emplace(tours.first.begin(), std::to_string(depotId));
        }
    } */

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