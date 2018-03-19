#ifndef GAUTILS_HPP
#define GAUTILS_HPP

#include <vector>

#include "Map.hpp"
#include "Population.hpp"
#include "Utils.hpp"
#include "Mutation.hpp"
#include "MutationUtils.hpp"
using namespace std;

vector<int> tourGen(Map& );
Population popGen(Map&, int );
void coutTour(vector<int>, Map&);
bool checkStuck(double bestFitK, double lastBestFit,int &stuckCount);
void applyCrossover(Population &,vector<vector<int>> &);
void applyMutation(Population &,Map &, int );
void initialPopApplyMutation(Population &,int ,Map &);
void disturb(Population &pop, Map &customerMap, int mutIterations,int disturbFactor);
#endif