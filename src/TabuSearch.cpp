//
// Created by aabedraba on 25/10/19.
//

#include "TabuSearch.h"

TabuSearch::TabuSearch(const Airport *airport) :
        _airport(airport),
        _sizeVectors(airport->getNumDoors()) {
}

TabuSearch::~TabuSearch() {

}

void TabuSearch::generateSolution(vector<int>& solutionVector) {
    int iterationsInSolution = 0, movements = 0;
    vector<int> iterVector = solutionVector;
    int iterVectorCost = _solutionCost = Utils::solutionCost(solutionVector, _airport->getFluxMatrix(),
                                                             _airport->getDistanceMatrix(),
                                                             _airport->isSimetric());
    pair<int, int> bestSwap;
    //TODO PARAMETRIZAR
    while (movements < 500) {
        while (iterationsInSolution < 100) {
            int bestIterationCost = bestNeighbour(bestSwap, iterVector, iterVectorCost);
            swap(iterVector[bestSwap.first], iterVector[bestSwap.second]);
            if (_shortTermMemory.size() == 5) _shortTermMemory.pop_front();
            _shortTermMemory.push_back(bestSwap);
            iterVectorCost = bestIterationCost;
            movements++;
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
    while (solutionsGenerated < 10) {
        do {
            swapPositions.first = rand() % _sizeVectors;
            swapPositions.second = rand() % _sizeVectors;
            if (swapPositions.first < swapPositions.second) swap(swapPositions.first, swapPositions.second);
        } while (swapPositions.first == swapPositions.second || isTabu(swapPositions));
        solutionsGenerated++;
        int swapCost = Utils::calculateSwapCost(iterVector, iterVectorCost, swapPositions,
                                                _airport->getFluxMatrix(), _airport->getDistanceMatrix(),
                                                _airport->isSimetric());
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

