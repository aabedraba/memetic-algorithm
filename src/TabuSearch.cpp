//
// Created by aabedraba on 25/10/19.
//

#include "TabuSearch.h"

TabuSearch::TabuSearch(const Airport *airport) :
        _airport(airport),
        _sizeVectors(airport->getNumDoors()),
        _longTermMemory(airport->getNumDoors(), vector<int>(airport->getNumDoors())) {
    auto start = std::chrono::steady_clock::now();
    initializeRandomSolution();
//    Utils::logSolution("Initial Solution", make_pair(-1, -1), _solutionVector, _solutionCost, 0, -1);
    generateSolution();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<float> diff = end - start;
    _executionTime = diff.count();
}

TabuSearch::~TabuSearch() {

}

void TabuSearch::initializeRandomSolution() {
    for (int i = 0; i < _sizeVectors; ++i)
        _solutionVector.push_back(i);

    for (int i = 0; i < _sizeVectors; ++i) {
        int randInRange = rand() % _sizeVectors;
        swap(_solutionVector[i], _solutionVector[randInRange]);
    }
    _solutionCost = Utils::solutionCost(_solutionVector, _airport->getFluxMatrix(), _airport->getDistanceMatrix(),
                                        _airport->isSimetric());
}

void TabuSearch::generateSolution() {
    int iterationsInSolution = 0, movements = 0, movesEnvironment = 0;
    vector<int> iterVector = _solutionVector;
    int iterVectorCost = _solutionCost;
    pair<int, int> bestSwap;
    int iterationToSave;
    while (movements < 50000) {
        while (iterationsInSolution < 100) {
            int bestIterationCost = bestNeighbour(bestSwap, iterVector, iterVectorCost);
            swap(iterVector[bestSwap.first], iterVector[bestSwap.second]);
            if (_shortTermMemory.size() == 5) _shortTermMemory.pop_front();
            _shortTermMemory.push_back(bestSwap);
            _longTermMemory[bestSwap.first][bestSwap.second]++;
            _longTermMemory[bestSwap.second][bestSwap.first]++;
            iterVectorCost = bestIterationCost;
            movements++;
            //_log += Utils::logSolution("Solution", bestSwap, iterVector, iterVectorCost, movements,
//                                       movesEnvironment);
            if (iterVectorCost < _solutionCost) {
                _solutionVector = iterVector;
                _solutionCost = iterVectorCost;
                iterationToSave = iterationsInSolution;
                iterationsInSolution = 0;
            } else {
                iterationsInSolution++;
            }
        }
        movesEnvironment++;
        iterVectorCost = generateEnvironment(iterVector);
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

int TabuSearch::generateEnvironment(vector<int> &iterVector) {
    for (int k = 0; k < _sizeVectors; ++k) {
        iterVector[k] = 0;
    }
    bool diversify = false;
    int random = rand() % 2;
    if (random) diversify = true;
    vector<int> generationResult;
    int candPos = 0, candidate;
    for (int i = 0; i < _sizeVectors; ++i) {
        if (diversify) candidate = INT_MAX;
        else candidate = INT_MIN;
        for (int j = 0; j < _sizeVectors; ++j) {
            bool isBetterCandidate = candidateIsBetter(i, j, candidate, diversify);
            bool alreadyInserted = candidateIsInserted(generationResult, j);
            if (isBetterCandidate && !alreadyInserted) {
                candidate = _longTermMemory[i][j];
                candPos = j;
            }
        }
        generationResult.push_back(candPos);
    }
    iterVector = generationResult;
    int cost = Utils::solutionCost(iterVector, _airport->getFluxMatrix(), _airport->getFluxMatrix(),
                                   _airport->isSimetric());
    return cost;
}

bool TabuSearch::candidateIsBetter(const int row, const int column, const int candidate, const bool diversify) {
    if (diversify) return _longTermMemory[row][column] < candidate;
    else return _longTermMemory[row][column] > candidate;
}

bool TabuSearch::candidateIsInserted(const vector<int> &partialGenResult, const int candidate) {
    for (int i = 0; i < partialGenResult.size(); ++i) {
        if (candidate == partialGenResult[i])
            return true;
    }
    return false;
}

int TabuSearch::getSolutionCost() const {
    return _solutionCost;
}

const vector<int> &TabuSearch::getSolutionVector() const {
    return _solutionVector;
}

//const string TabuSearch::getLog() const {
//    return Utils::getLog("Tabu", _log, _executionTime, _airport->getAirportName(), _sizeVectors, _solutionVector,
//                         _solutionCost);
//}

