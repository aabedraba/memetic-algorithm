//
// Created by aabedraba on 6/11/19.
//

#ifndef INC_2_GENETIC_ALGORITHMS_GENETIC_H
#define INC_2_GENETIC_ALGORITHMS_GENETIC_H

#include <vector>
#include <tuple>
#include "Airport.h"
#include "Utils.h"
#include "TabuSearch.h"

using namespace std;

class Memetic {
public:
    Memetic(const Airport *airport, TabuSearch *tabu, const string crossoverType,
            const unsigned int numEvaluations, const int populationToEvolve, const int crossOverChance,
            const int mutationChance, const int tournamentSize);

    virtual ~Memetic();

    const string &getLog() const;

    const Airport *getAirport() const;

    const Chromosome &getBestSolution() const;

    double getExecutionTime() const;

    int getBestCost() const;

private:
    void generateSolution();

    void initializeIndividualRandomly(Individual &individual);

    void selection();

    void cross();

    void mutation();

    void selectRabble(vector<int> &rabblePositions, const int numRabbles);

    void reEval();

    void replacement();

    bool checkIfSame(const Chromosome &first, const Chromosome &second);

    void ox2Cross(const Chromosome &first, Chromosome &second);

    void mocCross(const int cut, const Chromosome &first, Chromosome &second);

private:
    const Airport *_airport;
    const string _crossOverType;
    const int _popSize;
    const int _chromosomeSize;
    const int _populationToEvolve;
    const int _crossOverChance;
    const int _tournamentSize;
    const int _mutationChance;
    TabuSearch *_tabu;

    Population _initialPopulation;
    Population _currentPopulation;
    Chromosome _bestSolution;

    int _numEvaluations;
    int _totalEvaluations;
    string _log;
    double _executionTime;
    int _bestCost;
};


#endif //INC_2_GENETIC_ALGORITHMS_GENETIC_H
