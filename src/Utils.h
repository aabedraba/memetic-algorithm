//
// Created by aabedraba on 6/11/19.
//

#ifndef INC_2_GENETIC_ALGORITHMS_UTILS_H
#define INC_2_GENETIC_ALGORITHMS_UTILS_H

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

typedef vector<int> Chromosome;
struct Individual {
    Chromosome chromosome;
    unsigned int cost;
    bool evaluated;
};
typedef vector<Individual> Population;

namespace Utils {
    int solutionCost(const vector<int> &solution, const vector<vector<int>> &fluxMatrix,
                     const vector<vector<int>> &distanceMatrix, const bool simetric);

    void logPopulation(Population &population, int generation, string &log);

    void logCross(string position, const Chromosome *first, const Chromosome *second, string &log);

    void logVector(string position, const vector<int> *vector, string &log);

    void logElitesAndRabbles(Population *initialPopulation, Population *currentPopulation, vector<int> &elitePos,
                             vector<int> &rabblePos, string &log);

    void logBestSolution(const Chromosome *solution, int cost, string &log);

    int posMaxElement(const vector<pair<int, int>> &vector);

    int posMinElement(const vector<pair<int, int>> &vector);

    void writeInFile(int seed, const string type, const string airport, const string &log);

    void getResults(string type, string airportName, int bestCost, const vector<int>& bestSolution, double executionTime, string &log);


    int partialSwapCost(const vector<int> &solution, const pair<int, int> swapPositions,
                        const vector<vector<int>> &fluxMatrix, const vector<vector<int>> &distanceMatrix,
                        const bool simetric);
    struct SwapCostFunctor{
    public:
        SwapCostFunctor(const vector<vector<int>> &fluxMatrix,
                        const vector<vector<int>> &distanceMatrix, const bool simetric):
                        _fluxMatrix(fluxMatrix), _distanceMatrix(distanceMatrix),
                        _simetric(simetric)
        {};
        int operator()(vector<int> &solution, const int solutionCost, const pair<int, int> &swapPos){
            int preSwapCost = 0, afterSwapCost = 0;
            preSwapCost = partialSwapCost(solution, swapPos, _fluxMatrix,
                                          _distanceMatrix,
                                          _simetric);
            swap(solution[swapPos.first], solution[swapPos.second]);
            afterSwapCost = partialSwapCost(solution, swapPos, _fluxMatrix,
                                            _distanceMatrix,
                                            _simetric);
            swap(solution[swapPos.first], solution[swapPos.second]);
            return solutionCost + afterSwapCost - preSwapCost;
        }
    private:
        const vector<vector<int>> _fluxMatrix;
        const vector<vector<int>> _distanceMatrix;
        const bool _simetric;

    };
//    const string getLog(string algorithmType, const string log, float time, string airportName, int sizeVectors, const vector<int> &solutionVector,
//                        int solutionCost);
//    string logSolution(string type, pair<int, int> movement, vector<int> &solutionVector, int cost, int iteration, int environmentIteration);

}


#endif //INC_2_GENETIC_ALGORITHMS_UTILS_H
