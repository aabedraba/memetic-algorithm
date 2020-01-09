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
    TabuSearch(const Airport *airport);
    virtual ~TabuSearch();

private:
    void initializeRandomSolution();
    void generateSolution();

    int bestNeighbour(pair<int, int> &bestSwap, vector<int> &iterVector, const int &iterVectorCost);

    bool isTabu(pair<int, int> &swapElements );

    int generateEnvironment(vector<int> &iterVector);
    bool candidateIsBetter( const int row, const int column, const int candidate, const bool diversify );
    bool candidateIsInserted(const vector<int> &partialGenResult, const int candidate);
    //void logSolution(string type, pair<int, int> movement, vector<int> &solutionVector, int cost, int iteration, int environmentSolution);

public:
    int getSolutionCost() const;
    const vector<int> &getSolutionVector() const;

    const string getLog() const;

private:
    Airport *_airport;
    int _sizeVectors;
    int _solutionCost;
    vector<int> _solutionVector;
    list<pair<int, int>> _shortTermMemory;
    vector<vector<int>> _longTermMemory;
    string _log;
    float _executionTime;
};


#endif //META_TABUSEARCH_H
