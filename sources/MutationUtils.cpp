#include "MutationUtils.hpp"
#include "TourUtils.hpp"
vector<int> doubleSwap(vector<int> tour,int currentValue,int iterationValue,int newValueU, 
int newValueX,int newValueV,int newValueY){
    vector<int> swappedTour=tour;
    swappedTour[currentValue]=newValueU;
    swappedTour[currentValue+1]=newValueX;
    swappedTour[iterationValue]=newValueV;
    swappedTour[iterationValue+1]=newValueY;
    return swappedTour;
}

bool checkInSameTour(vector<int> Tour, int depotId, int customer1,int customer2){
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

 void printVector(vector<int> tour){
    cout<< "Imprimindo Vetor"<<endl;
    for(auto it: tour){
        cout<<it<< " ";
    }
    cout<<endl;
    //cout<< "Fitness: "<<getTourFitness(tour,map)<<endl;
}

pair<vector<int>,bool> basicFitnessEvaluation(Tour OldTour,Tour NewTour){
    pair<vector<int>,bool> result;
    result.first=OldTour.getRoute();
    result.second=false;
    if(NewTour.getFitness()>OldTour.getFitness()){
        result.first=NewTour.getRoute();
        result.second=true;
    }
    return result;
}

bool validateTour(vector<int> tour, int depositNumber,int depositId){
    int valuesCont[tour.size()+1];
    for(unsigned i=0;i<tour.size();i++){
        valuesCont[i]=0;
    }
    for(auto value: tour){
        valuesCont[value]++;
    }

    for(unsigned i=0;i<=((tour.size()+1)-(depositNumber));i++){
        //cout<<"O valor "<<i<< " aparece "<< valuesCont[i] <<"vezes" <<endl;;
        if((i!=depositId && valuesCont[i]>1) ||(i!=0 && valuesCont[i]==0)){
            cout <<"Deu merda"<<endl;
            return false;
        }
    }
    return true;
}

pair<vector<int>,bool> setInitialResult(vector<int> tour){
    pair<vector<int>,bool> result;
    result.first=tour;
    result.second=false;
    return result;
}