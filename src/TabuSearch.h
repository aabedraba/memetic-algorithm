//
// Created by aabedraba on 25/10/19.
//

#ifndef META_TABUSEARCH_H
#define META_TABUSEARCH_H

#include "Airport.h"
#include "Utils.h"
#include <bits/stdc++.h>
#include <chrono>

class TabuSearch {
public:
    TabuSearch(const Airport *airport, const int maxGenerations, const int maxIterationsInSolution,
               const int tabuListSize, const int environmentsToGenerate);

    virtual ~TabuSearch();

private:
    int bestNeighbour(pair<int, int> &bestSwap, vector<int> &iterVector, const int &iterVectorCost);

    bool isTabu(pair<int, int> &swapElements);
    //void logSolution(string type, pair<int, int> movement, vector<int> &solutionVector, int cost, int iteration, int environmentSolution);

public:
    void generateSolution(vector<int> &solutionVector);

    int getSolutionCost() const;

    const string getLog() const;

private:
    const Airport *_airport;

    const int _maxGenerations;
    const int _maxIterationsInSolution;
    const int _tabuListSize;
    const int _environmentsToGenerate;
    Utils::SwapCostFunctor _swapCost;

    int _sizeVectors;
    int _solutionCost;
    vector<int> _solutionVector;
    list<pair<int, int>> _shortTermMemory;
    string _log;
    float _executionTime;

};


#endif //META_TABUSEARCH_H
