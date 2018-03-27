#include <algorithm>

#include "HamiltonianCycle.hpp"
#include "Configs.hpp"
#include "TourUtils.hpp"

HamiltonianCycle::correlationMap ranking;
string depotCopyToken{"'"};
unsigned correlationDuplicate{0};
unsigned correlationIn_Out{0};


bool validaMerdaChoosen(HamiltonianCycle::choosenSubs merda){

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
    correlationDuplicate = (red.getRoute().size()*0.2);
    correlationIn_Out = correlationDuplicate*3;

    vector<vector<int>> redSubs{explodeSubTours(red.getRoute(), Configs::customerMap.getDepotId())};
    vector<vector<int>> blueSubs{explodeSubTours(blue.getRoute(), Configs::customerMap.getDepotId())};

    choosenSubs choosen = mapDepots(redSubs, blueSubs);
    cout << "PreChoose"<<endl;
    for(auto it : choosen){
        cout << "Red " << it.first << " Blue " << it.second << endl;
    }
    
    choosen = validateChoosenSubs(choosen);

    //exit(EXIT_FAILURE);
    cout << "NEW CHOOSEN" << endl;
    for(auto it : choosen){
        cout << it.first << " " << it.second << endl;
    }


    if(choosen.size() != redSubs.size()){
        choosen = fillWithSecondOptions(choosen, redSubs.size());
    }

    std::sort(choosen.begin(), choosen.end(), [](auto &left, auto &right){ return(left.first < right.first); });

    cout << "FINAL CHOOSEN" << endl;
    for(auto it : choosen){
        cout << it.first << " " << it.second << endl;
    }

    if(!validaMerdaChoosen(choosen)){
        cout << "DEU MERDA NA CHOOSEN"<<endl;
        exit(0);
    }

    parentsHamiltonian newTours = rebuildTours(redSubs, blueSubs, choosen);

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
    ranking.clear();
    return(newTours);
}

// Will generate a list of pre selected combinations of subTours
HamiltonianCycle::choosenSubs HamiltonianCycle::mapDepots(subTours& redSubs, subTours& blueSubs){
    unsigned redSubsSize{redSubs.size()}, blueSubsSize{blueSubs.size()};

    choosenSubs preChoose;

    cout << "Size " << blueSubsSize << endl;
    for(unsigned r=0; r<redSubsSize; r++){
        cout << "---- RED ----" << endl;
        for(unsigned b=0; b<blueSubsSize; b++){
            ranking[r].push_back(Correlation(0, 0));
            unsigned redInsideSize{redSubs[r].size()}; 
            unsigned blueInsideSize{blueSubs[b].size()};
            cout << "SubRed: ";
            for(int c : redSubs[r]){
                cout << c <<" ";
            }cout << endl;
            cout << "SubBlue: ";
            for(int c : blueSubs[b]){
                cout << c <<" ";
            }
            cout << endl;
            for(unsigned rInside=0; rInside<redInsideSize; rInside++){
                cout << "Procurar " << redSubs[r][rInside] << " ";
                // return the position of element in vector, if return == size, the element not found in vector
                auto position = std::distance(blueSubs[b].begin(), std::find(blueSubs[b].begin(), blueSubs[b].end(), redSubs[r][rInside]));
                if(!(position>=blueInsideSize)){
                    cout << "achou ";
                    if((position==0 || position==(redInsideSize-1))&&(rInside==0 || rInside==(redInsideSize-1))){
                        cout << " InOut ";
                        ranking[r][b].score+=correlationIn_Out;
                        ranking[r][b].numberInOut++;
                    }
                    ranking[r][b].score+=correlationDuplicate;
               }
               cout << endl;
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

// Will validate chooses of Map, changing if necessary
HamiltonianCycle::choosenSubs HamiltonianCycle::validateChoosenSubs(choosenSubs preChoosen){
    choosenSubs equals;
    choosenSubs newChoosen;
    choosenSubs old{preChoosen};

    cout << "----------- CHOOSEN ----------" << endl;
    for(auto it : old){
        cout <<"Validate First " << it.first << " Second " << it.second << endl;
        equals = findEqualsChooses(preChoosen, it.second);
        cout << "PreChoosen Size " << preChoosen.size()<<endl;
        cout << "Equals Size " << equals.size() << endl;
        if(equals.size()>1){
            vector<ResolveScore> scores;
            for(auto eq : equals){
                scores.push_back(ResolveScore(eq.first, eq.second, ranking[eq.first][eq.second].score));
            }
            std::sort(scores.begin(), scores.end(), [](auto &right, auto &left){ return(left.score<right.score); });
            cout << "scoresSorted ";
            if(!scores.empty()){
                for(auto c : scores){
                    cout << c.score << " ";
                }
                cout << endl;
            }
            /* REEEEEEVEEEEER */
            // Get a list of first best repeated elements
            auto it = std::adjacent_find(scores.begin(), scores.end(), [](auto &right, auto &left) { return(right.score == left.score);});
            cout << "Achou algum adjacente? " << ((it==scores.end()) ? "Não" : "Sim") <<endl;
            if(it!=scores.end() && (*it).score == scores.front().score){
                // Desempate por nota, notas iguais
                cout << "Notas iguais, Fight N_IN_OUT"<<endl;
                unsigned i{0};
                int maxNinOut{-1}, subRedMaxNinOut{-1}, subBlueMaxNinOut{-1};
                unsigned limitWhile{scores.size()-1};
                while((*it).score == scores[i].score){ // várias notas repetidas
                    int thisInOut{ranking[scores[i].posRed][scores[i].posBlue].numberInOut};
                    if(maxNinOut < thisInOut){
                        cout << "MaxNin " << maxNinOut << " ThisInOut " << thisInOut << endl; 
                        maxNinOut = thisInOut;
                        subRedMaxNinOut = scores[i].posRed;
                        subBlueMaxNinOut = scores[i].posBlue;
                    }
                    i++;
                    if(i==limitWhile) break;
                }
                cout << "CHEGOU NO DESEMPATE POR IN_OUT"<< " RED " << subRedMaxNinOut << " BLUE " << subBlueMaxNinOut << endl;
                newChoosen.push_back(std::make_pair(subRedMaxNinOut, subBlueMaxNinOut));
                /* REEEEEEVEEEEER */
            }else {
                // Desempate por nota, primeiro venceu
                cout << "DESEMPATE POR NOTA"<<endl;
                newChoosen.push_back(std::make_pair(scores.front().posRed, scores.front().posBlue));
            }
            
        } else {
            cout << "Passou direto "<<  endl;
            if(!equals.empty()) newChoosen.push_back(equals[0]);
        }
    }
    return(newChoosen);
}

HamiltonianCycle::choosenSubs HamiltonianCycle::fillWithSecondOptions(choosenSubs choosen, const unsigned redSize){
    vector<int> redChoosen, blueChoosen;
    vector<int> redNotChoosen, blueNotChoosen;
    choosenSubs secondOptionsSubs{choosen};

    for(auto it : choosen){
        redChoosen.push_back(it.first);
        blueChoosen.push_back(it.second);
    }
    cout << endl << endl <<  "Choosen vector " << redChoosen.size() << " " << blueChoosen.size() << endl;
    cout << "Red ";
    for(int red : redChoosen){
        cout << red << " ";
    }
    cout << endl;
    cout << "blue ";
    for(int blue : blueChoosen){
        cout << blue << " ";
    }
    cout << endl << endl;
    for(unsigned i=0; i<redSize; i++){
        auto itRed = std::find(redChoosen.begin(), redChoosen.end(), i);
        if(itRed == redChoosen.end()) redNotChoosen.push_back(i);

        auto itBlue = std::find(blueChoosen.begin(), blueChoosen.end(), i);
        if(itBlue == blueChoosen.end()) blueNotChoosen.push_back(i);
    }
    cout << "NotChoosen vector"<<endl;
    cout << "Red ";
    for(int red : redNotChoosen){
        cout << red << " ";
    }
    cout << endl;
    cout << "blue ";
    for(int blue : blueNotChoosen){
        cout << blue << " ";
    }
    cout << endl << endl;
    /* for(int red : redNotChoosen){
        int maxScore{-1}, blueMaxScore{-1}, blueMaxPos{-1};
        for(unsigned blue=0; blue<blueNotChoosen.size(); blue++){
            cout << "Red " << red << " Blue " << blueNotChoosen[blue] << endl;
            cout << "Ranking " << ranking[red][blueNotChoosen[blue]].score << " " << maxScore << endl;
            if(ranking[red][blueNotChoosen[blue]].score > maxScore){
                cout << "Entrou"<<endl;
                maxScore = ranking[red][blueNotChoosen[blue]].score;
                blueMaxScore = blueNotChoosen[blue];
                blueMaxPos = blue;
            }
        }
        cout << "MAxSCORE" << blueMaxScore << " POS " << blueMaxPos << endl;
        secondOptionsSubs.push_back(std::make_pair(red, blueMaxScore));
        blueNotChoosen.erase(blueNotChoosen.begin()+blueMaxPos);
    } */

    vector<SecondResolve> secondResolve; 
    for(unsigned red=0; red<redNotChoosen.size(); red++){
        // int maxScore{-1}, blueMaxScore{-1}, blueMaxPos{-1};
        for(unsigned blue=0; blue<blueNotChoosen.size(); blue++){
            cout << "PosNot " << red << " " << blue<<endl;
            cout << "ValNot " << redNotChoosen[red] << " " << blueNotChoosen[blue]<<endl;
            cout << "Ranking " << ranking[red][blueNotChoosen[blue]].score<< endl; 
            secondResolve.push_back(SecondResolve(red, blue, redNotChoosen[red], blueNotChoosen[blue], ranking[redNotChoosen[red]][blueNotChoosen[blue]].score));
        }
    }
    std::sort(secondResolve.begin(), secondResolve.end(), [](auto &left, auto &right){ return(left.score > right.score); });

    // Navegar pelo vetor da struct, ir dando push nos elementos que estiverem no vector blue not choosen, apagar após push
    // ABUSAR DA FIND
    cout << "Após o sort"<<endl;
    for(auto it : secondResolve){

        vector<int>::iterator findRedNotChoosen{std::find(redNotChoosen.begin(), redNotChoosen.end(), it.subRed)};
        vector<int>::iterator findBlueNotChoosen{std::find(blueNotChoosen.begin(), blueNotChoosen.end(), it.subBlue)};

        bool inBlueNotChoosen{findBlueNotChoosen != blueNotChoosen.end()};
        bool inRedNotChoosen{findRedNotChoosen != redNotChoosen.end()};

        if(inBlueNotChoosen && inRedNotChoosen){
            secondOptionsSubs.push_back(std::make_pair(it.subRed, it.subBlue));
            cout << "Pos " << it.posRed << " " << it.posBlue <<endl;
            cout << "Val " << it.subRed << " " << it.subBlue << endl;
            cout << "Size " << blueNotChoosen.size() << " " << redNotChoosen.size() << endl;

            blueNotChoosen.erase(findBlueNotChoosen);
            redNotChoosen.erase(findRedNotChoosen);

            cout << "?" << endl;
        }
    }
    cout << "MAAAAP "<<endl;
    for(auto it : secondResolve){
        // cout << "POS " << it.posRed << " " << it.posBlue << endl;
        cout << "Red " << it.subRed << " Blue " << it.subBlue << " Score " << it.score << endl;
    }
    return(secondOptionsSubs);
}

// Rebuild tour with subs Chooses
HamiltonianCycle::parentsHamiltonian HamiltonianCycle::rebuildTours(subTours& redTours, subTours& blueTours, choosenSubs& choosen) {
    int depotId{Configs::customerMap.getDepotId()};
    parentsHamiltonian tours;

/*     cout << "RedSubs"<<endl;
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
    } */

    cout << "================ REBUILD TOURS ======================"<<endl;
    for(auto it : choosen){
        tours.first.push_back(std::to_string(depotId));
        tours.second.push_back(std::to_string(depotId));
        for(int c : redTours[it.first]){
            tours.first.push_back(std::to_string(c));
        }
        for(int c : blueTours[it.second]){
            tours.second.push_back(std::to_string(c));
        }
    }
    vector<int> choosenReds;
    vector<int> choosenBlues;
    for(auto it : choosen){
        choosenReds.push_back(it.first);
        choosenBlues.push_back(it.second);
    }
    std::sort(choosenReds.begin(), choosenReds.end(), [](int &left, int &right){ return(left>right); });
    std::sort(choosenBlues.begin(), choosenBlues.end(), [](int &left, int &right){ return(left>right); });

    for(int i=0; i<choosen.size(); i++){
        // cout << "Erase Red " << choosenReds[i] << " Blue " << choosenBlues[i] << endl;
        redTours.erase(redTours.begin()+choosenReds[i]);
        blueTours.erase(blueTours.begin()+choosenBlues[i]);
    }

    if(!redTours.empty() && !blueTours.empty()){
        for(auto it : redTours){
            tours.first.push_back(std::to_string(depotId));
            for(int c : it){
                tours.first.push_back(std::to_string(c));
            }
        }
        for(auto it : blueTours){
            tours.second.push_back(std::to_string(depotId));
            for(int c : it){
                tours.second.push_back(std::to_string(c));
            }
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

// return a list with equals combinations of red->blue
HamiltonianCycle::choosenSubs HamiltonianCycle::findEqualsChooses(choosenSubs& choosen, int element){
    choosenSubs equals;
    unsigned size{choosen.size()};
    vector<int> pos;
    for(unsigned i=0; i<size; i++){
        if(choosen[i].second == element){
            pos.push_back(i);
            equals.push_back(choosen[i]);
        }
    }
    for(unsigned i=pos.size(); i>0; i--){
        choosen.erase(choosen.begin()+pos[i-1]);
    }
    return(equals);
}

