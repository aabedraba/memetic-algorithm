//
// Created by aabedraba on 25/10/19.
//

#include "TabuSearch.h"

TabuSearch::TabuSearch(const Airport *airport, const int maxGenerations, const int maxIterationsInSolution,
                       const int tabuListSize, const int environmentsToGenerate
) :
        _airport(airport),
        _sizeVectors(airport->getNumDoors()),
        _maxGenerations(maxGenerations),
        _maxIterationsInSolution(maxIterationsInSolution),
        _tabuListSize(tabuListSize),
        _environmentsToGenerate(environmentsToGenerate),
        _swapCost(airport->getFluxMatrix(), airport->getDistanceMatrix(), airport->isSimetric())
        {
}

TabuSearch::~TabuSearch() {

}

void TabuSearch::generateSolution(vector<int> &solutionVector) {
    int iterationsInSolution = 0, generations = 0;
    vector<int> iterVector = solutionVector;
    int iterVectorCost = _solutionCost = Utils::solutionCost(solutionVector, _airport->getFluxMatrix(),
                                                             _airport->getDistanceMatrix(),
                                                             _airport->isSimetric());
    pair<int, int> bestSwap;
    while (generations < _maxGenerations) {
        while (iterationsInSolution < _maxIterationsInSolution) {
            int bestIterationCost = bestNeighbour(bestSwap, iterVector, iterVectorCost);
            swap(iterVector[bestSwap.first], iterVector[bestSwap.second]);
            if (_shortTermMemory.size() == _tabuListSize) _shortTermMemory.pop_front();
            _shortTermMemory.push_back(bestSwap);
            iterVectorCost = bestIterationCost;
            generations++;
            if (iterVectorCost < _solutionCost) {
                solutionVector = iterVector;
                _solutionCost = iterVectorCost;
                iterationsInSolution = 0;
            } else {
                iterationsInSolution++;
            }
        }
        iterationsInSolution = 0;
    }
}

int TabuSearch::bestNeighbour(pair<int, int> &bestSwap, vector<int> &iterVector, const int &iterVectorCost) {
    pair<int, int> swapPositions;
    int bestSwapCost = INT_MAX;
    int solutionsGenerated = 0;
    while (solutionsGenerated < _environmentsToGenerate) {
        do {
            swapPositions.first = rand() % _sizeVectors;
            swapPositions.second = rand() % _sizeVectors;
            if (swapPositions.first < swapPositions.second) swap(swapPositions.first, swapPositions.second);
        } while (swapPositions.first == swapPositions.second || isTabu(swapPositions));
        solutionsGenerated++;
        int swapCost = _swapCost(iterVector, iterVectorCost, swapPositions);
        if (swapCost < bestSwapCost) {
            bestSwap = swapPositions;
            bestSwapCost = swapCost;
        }
    }
    return bestSwapCost;
}

bool TabuSearch::isTabu(pair<int, int> &swapElements) {
    for (auto it = _shortTermMemory.begin(); it != _shortTermMemory.end(); ++it)
        if (swapElements == *it)
            return true;
    return false;
}

int TabuSearch::getSolutionCost() const {
    return _solutionCost;
}


//const string TabuSearch::getLog() const {
//    return Utils::getLog("Tabu", _log, _executionTime, _airport->getAirportName(), _sizeVectors, _solutionVector,
//                         _solutionCost);
//}

