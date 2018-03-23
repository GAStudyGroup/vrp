#include "Mutation.hpp"
#include "Tour.hpp"
#include "TourUtils.hpp"
#include "Configs.hpp"

vector<int> Mutation::evaluateMutation(vector<int> tour){
    pair<vector<int>,bool> result;
    vector<int> initial = tour;
    int it=0;
    for (auto value: tour){
       result=firstMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhorou primeiro mov"<<endl;
           return result.first;
        }
       result=secondMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhorou segundo mov"<<endl;
            return result.first;
        }
        result=thirdMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhorou terceiro mov"<<endl;
            return result.first;
        }

        result=fourthMove(initial,it,value); //Funfando
        if(result.second==true){
            //cout<<"Melhorou quarto mov"<<endl;
            return result.first;
        }
        
        result=fifthMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhor quinto mov"<<endl;
            return result.first;
        }
        result=sixthMove(initial,it,value); //Funfando
        if(result.second==true){
            //cout<<"Melhor sexto mov"<<endl;
            return result.first;
        }
        result=seventhMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhor seventh mov"<<endl;
           return result.first; 
        }
        result=eighthMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhor seventh mov"<<endl;
           return result.first; 
        }
        result=ninethMove(initial,it,value);
        if(result.second==true){
            //cout<<"Melhor seventh mov"<<endl;
           return result.first; 
        }
        it++;
    }  
    return tour;
 /* pair<vector<int>,bool> result;
    vector<int> initial = tour;
    int it=0;
    for (auto value: tour){
        for(auto move: getMoves()){
            result=move(initial,it,value);
            if(result.second){
                return result.first;
            }
        }
        it++;
    }  
    return tour;*/
}

    pair<vector<int>,bool> Mutation::firstMove(vector<int> tour,int it,int value){
    vector<int> initial=tour; 
    pair<vector<int>,bool> result=setInitialResult(tour);
    //cout<<"Fitness antes do movimento "<< iFitness <<endl;
    if(value!=Configs::customerMap.getDepotId()){ //Only does the move it isn't a depot   
        for(unsigned int i=0;i<initial.size()-1;i++){
            if(initial[i]==value){
                continue;
            }else{
                initial.erase(find(initial.begin(),initial.end(),value));
                initial.emplace(initial.begin()+i,value);                
            }
            //printVector(initial); //Debug
            result=basicFitnessEvaluation(Tour(tour),Tour(initial));
            // printVector(initial); //Debug
            if(result.second){
                break;
            }
        }                
    }else{
        result.first=initial;
        result.second=false;
    }
    //cout<<"Fitness depois do movimento "<<bFitness<<endl;
    return result;
}



pair<vector<int>,bool> Mutation::secondAndThirdMove(vector<int> tour, int it,int value,int mode){
    //cout<<"iterator: " <<it <<endl;
    pair<vector<int>,bool> result=setInitialResult(tour);
    int valU=0;
    int valX=0;
    if(it==Configs::customerMap.getDepotId() || it==(int)tour.size()-1){
        return result;
    }else{
        for(unsigned int i=0;i<result.first.size()-1;i++){
                valU=result.first[i];
                valX=result.first[i+1];
                //cout<<"VALU: "<<valU << " VALX: "<< valX<< " IT: "<<it<<endl;
                if(valU==Configs::customerMap.getDepotId() || valX==Configs::customerMap.getDepotId() || valU==value || valX==value){
                    continue;
                }
                result.first.erase(find(result.first.begin(),result.first.end(),valU));
                result.first.erase(find(result.first.begin(),result.first.end(),valX));
                if(mode==2){
                    result.first.emplace(result.first.begin()+it,valU);
                    result.first.emplace(result.first.begin()+(it+1),valX);    
                }else{
                    result.first.emplace(result.first.begin()+it,valX);
                    result.first.emplace(result.first.begin()+(it+1),valU);    
                }
                
                // /printVector(result.first);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                    return result;
                }
                result.first=tour;
        }
        result.second=false;
        return  result;
    }
}
pair<vector<int>,bool> Mutation::secondMove(vector<int> tour, int it,int value){
    return(secondAndThirdMove(tour,it,value,2));
}

pair<vector<int>,bool> Mutation::thirdMove(vector<int> tour, int it,int value){
    return(secondAndThirdMove(tour,it,value,3));
}
pair<vector<int>,bool> Mutation::fourthMove(vector<int> tour, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int auxId=0;
    if(value!=Configs::customerMap.getDepotId()){
        for(unsigned int i=0; i<tour.size();i++){
            if(result.first[i]!=Configs::customerMap.getDepotId() && result.first[i]!=value){
                auxId=result.first[i];
                result.first[i]=value;
                result.first[it]=auxId;
                //printVector(result.first);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                    return result;
                }
                result.first=tour;
            }
        
        }
    }
        return result;
}


pair<vector<int>,bool> Mutation::fifthMove(vector<int> tour, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    if(it<(int)tour.size()-1){
        int valU=0;
        int valX=0;
        if(value!=Configs::customerMap.getDepotId()){
            for(unsigned int i=0;i<tour.size()-1;i++){
                valU=result.first[i];
                valX=result.first[i+1];
                if(valU == Configs::customerMap.getDepotId() || valX == Configs::customerMap.getDepotId() ||valU==value ||valX==value){
                 continue;
                }
                //cout<< "Value U: "<<valU << " Value X: "<<valX<<endl;
                //cout<< "Result: "<<result.first[it]<<endl;

                result.first[it]=-result.first[it];
                
                result.first.emplace(find(result.first.begin(),result.first.end(),valU),value);

                result.first.erase(find(result.first.begin(),result.first.end(),valU));
                result.first.erase(find(result.first.begin(),result.first.end(),valX));

                result.first.emplace(find(result.first.begin(),result.first.end(),-value),valU);
                result.first.emplace(find(result.first.begin(),result.first.end(),-value),valX);

                result.first.erase(find(result.first.begin(),result.first.end(),-value));
                //printVector(result.first);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                    return result;
                }
                result.first=tour;
            }
        }
    }
    return result;
}

pair<vector<int>,bool> Mutation::sixthMove(vector<int> tour, int it,int value){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int valU=0,valX=0,valV=0,valY=0;
    if(value!=Configs::customerMap.getDepotId() && it<(int)(tour.size()-1)){
        valU=result.first[it];
        valX=result.first[it+1];
        for(unsigned int i=0; i< (tour.size()-1);i++){
            valV=result.first[i];
            valY=result.first[i+1];
            if(valV!=Configs::customerMap.getDepotId() && valY!=Configs::customerMap.getDepotId() && valX!=Configs::customerMap.getDepotId() 
            && valU!=valV && valU!=valX && valU!=valY 
            && valX!=valV && valX!=valY
            && valY!=valV){
                result.first=doubleSwap(result.first,it,i,valV,valY,valU,valX);
                result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                if(result.second){
                   // cout << "ValU: "<<valU << " ValX: "<<valX<<" ValV: "<<valV<<" ValY: "<<valY<<endl; 
                   // printVector(result.first);
                    return result;
                }
                result.first=tour;
            }
        }
    }
    return result;
}

pair<vector<int>,bool> Mutation::seventhAndEighthAndNineMove(vector<int> tour, int it,int value, int move){
    pair<vector<int>,bool> result=setInitialResult(tour);
    int ValU=0,ValX=0,ValV=0,ValY=0;
        if(value==Configs::customerMap.getDepotId() || it==(int)tour.size()-1){
            return result;
        }else{
            ValU=result.first[it];
            ValX=result.first[it+1];
            if(ValX!=ValU && ValX!=Configs::customerMap.getDepotId()){
                for(unsigned int i=0; i<tour.size()-1;i++){
                    ValV=result.first[i];
                    ValY=result.first[i+1];
                    if(ValX==Configs::customerMap.getDepotId()||ValV==Configs::customerMap.getDepotId() || ValY==Configs::customerMap.getDepotId() || ValU==ValV 
                    ||!(ValU!=ValV && ValU!=ValX && ValU!=ValY && ValX!=ValV && ValX!=ValY && ValY!=ValV)){
                        continue;
                    }
                    if(move==7){
                        if(!checkInSameTour(tour,Configs::customerMap.getDepotId(),ValU,ValV)){
                            continue;
                        }
                     }
                     else if(move==8 || move==9){
                        if(checkInSameTour(tour,Configs::customerMap.getDepotId(),ValU,ValV)){
                            continue;
                        }
                    }
                    if(move==7 || move==8){
                        result.first=doubleSwap(result.first,it,i,ValU,ValV,ValX,ValY);
                    }
                    else if(move==9){
                        result.first=doubleSwap(result.first,it,i,ValU,ValY,ValX,ValV);
                    }
                    result=basicFitnessEvaluation(Tour(tour),Tour(result.first));
                    if(result.second){
                        return result;
                    }
                }
            }
        }

    return result;
}
pair<vector<int>,bool> Mutation::seventhMove(vector<int> tour, int it,int value){
    return seventhAndEighthAndNineMove(tour,it,value,7);
}
pair<vector<int>,bool> Mutation::eighthMove(vector<int> tour, int it,int value){
    return seventhAndEighthAndNineMove(tour,it,value,8);
}
pair<vector<int>,bool> Mutation::ninethMove(vector<int> tour, int it,int value){
    return seventhAndEighthAndNineMove(tour,it,value,9);
}

vector<int> Mutation::doubleSwap(vector<int> tour,int currentValue,int iterationValue,int newValueU, 
    int newValueX,int newValueV,int newValueY){
    vector<int> swappedTour=tour;
    swappedTour[currentValue]=newValueU;
    swappedTour[currentValue+1]=newValueX;
    swappedTour[iterationValue]=newValueV;
    swappedTour[iterationValue+1]=newValueY;
    return swappedTour;
}

bool Mutation::checkInSameTour(vector<int> Tour, int depotId, int customer1,int customer2){
    vector<vector<int>> tours=explodeSubTours(Tour,depotId);
    bool flag1=false, flag2=false;
    for(auto tour: tours){
        for(auto customer:tour){
            if(customer==customer1){
                flag1=true;
            }
            if(customer==customer2){
                flag2=true;
            }
            if(flag1 && flag2){
                return true;
            }
        }
        flag1=false;
        flag2=false;
    }
    return false;
}

pair<vector<int>,bool> Mutation::basicFitnessEvaluation(Tour OldTour,Tour NewTour){
    pair<vector<int>,bool> result;
    result.first=OldTour.getRoute();
    result.second=false;
    if(NewTour.getFitness()>OldTour.getFitness()){
        result.first=NewTour.getRoute();
        result.second=true;
    }
    return result;
}

bool Mutation::validateTour(vector<int> tour, int depositNumber,int depositId){
    int valuesCont[tour.size()+1];
    for(unsigned i=0;i<tour.size();i++){
        valuesCont[i]=0;
    }
    for(auto value: tour){
        valuesCont[value]++;
    }

    for(unsigned i=0;i<=((tour.size()+1)-(depositNumber));i++){
        //cout<<"O valor "<<i<< " aparece "<< valuesCont[i] <<"vezes" <<endl;;
        if((i!=(unsigned)depositId && valuesCont[i]>1) ||(i!=0 && valuesCont[i]==0)){
            cout <<"Deu merda"<<endl;
            return false;
        }
    }
    return true;
}

pair<vector<int>,bool> Mutation::setInitialResult(vector<int> tour){
    pair<vector<int>,bool> result;
    result.first=tour;
    result.second=false;
    return result;
}
