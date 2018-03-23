#ifndef MUTATIONUTILS_H
#define MUTATIONUTILS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Tour.hpp"


using namespace std;
bool checkInSameTour(vector<int>, int,int,int);
pair<vector<int>,bool> basicFitnessEvaluation(Tour ,Tour);
void printVector(vector<int>);
bool validateTour(vector<int>,int,int);
pair<vector<int>,bool> setInitialResult(vector<int>);
vector<int> doubleSwap(vector<int>,int currentValue,int iterationValue,
int newValueU, int newValueX,int newValueV,int newValueY);


#endif