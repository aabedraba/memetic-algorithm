//
// Created by aabedraba on 6/11/19.
//

#include <algorithm>
#include <chrono>
#include "Memetic.h"
#include "TabuSearch.h"

Memetic::Memetic(const Airport *airport, TabuSearch *tabu, const string crossoverType,
                 const unsigned int numEvaluations, const int populationToEvolve, const int crossOverChance,
                 const int mutationChance, const int tournamentSize) :
        _airport(airport),
        _crossOverType(crossoverType),
        _numEvaluations(0),
        _popSize(50),
        _initialPopulation(_popSize),
        _currentPopulation(_populationToEvolve),
        _totalEvaluations(numEvaluations),
        _tabu(tabu),
        _chromosomeSize(airport->getNumDoors()),
        _populationToEvolve(populationToEvolve),
        _crossOverChance(crossOverChance),
        _mutationChance(mutationChance),
        _tournamentSize(tournamentSize) {
    for (int i = 0; i < _popSize; ++i) {
        _initialPopulation[i].chromosome.resize(_chromosomeSize);
    }
    generateSolution();

}

Memetic::~Memetic() {

}

void Memetic::generateSolution() {
    auto start = std::chrono::steady_clock::now();
    for (int ind = 0; ind < _popSize; ++ind)
        initializeIndividualRandomly(_initialPopulation[ind]);
    int generation = 1;
    while (_numEvaluations < _totalEvaluations) {
        Utils::logPopulation(_initialPopulation, generation, _log);
        selection();
        cross();
        mutation();
        if (_numEvaluations % 50 == 0) {
            for (int i = 0; i < _populationToEvolve; ++i) {
                _tabu->generateSolution(_currentPopulation[i].chromosome);
                _currentPopulation[i].cost = _tabu->getSolutionCost();
                _currentPopulation[i].evaluated = true;
                _numEvaluations++;
            }
        }
        reEval();
        replacement();
        generation++;
    }
    int posBestCost = 0;
    for (int i = 1; i < _popSize; ++i) {
        if (_initialPopulation[i].cost < _initialPopulation[posBestCost].cost)
            posBestCost = i;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<float> diff = end - start;
    string log = "________Best individual________\n\n";
    Utils::logBestSolution(&_initialPopulation[posBestCost].chromosome, _initialPopulation[posBestCost].cost, log);
    log += "\n";
    log += "Time: " + to_string(diff.count());
    log += "\n----------------------------------\n\n";
    _log = log + _log;

    _executionTime = diff.count();
    _bestSolution = _initialPopulation[posBestCost].chromosome;
    _bestCost = _initialPopulation[posBestCost].cost;
}

void Memetic::cross() {
    _log += "________Crosses________\n\n";
    _log += "Cross individual " + to_string(0) + "\n";
    for (int i = 0; i < _populationToEvolve / 2; i += 2) {
        int chance = rand() % 100;
        if (chance > _crossOverChance) {
            Utils::logCross("Parent", &_currentPopulation[0].chromosome, &_currentPopulation[0 + 1].chromosome, _log);
            _currentPopulation[0].evaluated = _currentPopulation[0 + 1].evaluated = false;
            if (_crossOverType == "ox2") {
                Chromosome copy = _currentPopulation[0 + 1].chromosome;
                ox2Cross(_currentPopulation[0].chromosome, _currentPopulation[0 + 1].chromosome);
                ox2Cross(copy, _currentPopulation[0].chromosome);
            } else if (_crossOverType == "moc") {
                int cut = _chromosomeSize - 2;
                Chromosome copy = _currentPopulation[0 + 1].chromosome;
                mocCross(cut, _currentPopulation[0].chromosome, _currentPopulation[0 + 1].chromosome);
                mocCross(cut, copy, _currentPopulation[0].chromosome);
            }
            Utils::logCross("Children", &_currentPopulation[0].chromosome, &_currentPopulation[0 + 1].chromosome, _log);
            _log += "\n\n";
        }
    }
}

void Memetic::selection() {
    for (int i = 0; i < _populationToEvolve; ++i) {
        vector<int> rands;
        while (rands.size() < _tournamentSize) {
            int toInsert = rand() % _popSize;
            auto iter = find(rands.begin(), rands.end(), toInsert);
            if (iter == rands.end()) {
                rands.push_back(toInsert);
            }
        }
        auto iter = rands.begin();
        int best = *iter;
        while (iter != rands.end()) {
            if (_initialPopulation[*iter].cost > _initialPopulation[best].cost)
                best = *iter;
            iter++;
        }
        _currentPopulation[i] = _initialPopulation[best];
    }
}

void Memetic::initializeIndividualRandomly(Individual &individual) {
    for (int i = 0; i < individual.chromosome.size(); ++i)
        individual.chromosome[i] = i;

    for (int i = 0; i < individual.chromosome.size(); ++i) {
        int randInRange = rand() % individual.chromosome.size();
        swap(individual.chromosome[i], individual.chromosome[randInRange]);
    }
    individual.cost = Utils::solutionCost(individual.chromosome, _airport->getFluxMatrix(),
                                          _airport->getDistanceMatrix(),
                                          _airport->isSimetric());
    individual.evaluated = true;
}

void Memetic::ox2Cross(const Chromosome &first, Chromosome &second) {
    vector<int> firstSelectedGenes;
    vector<int> posSecondGenes;
    int chance;
    for (int i = 0; i < _chromosomeSize; ++i) {
        chance = rand() % 100;
        if (chance > 50)
            firstSelectedGenes.push_back(first[i]);
    }
    for (int i = 0; i < firstSelectedGenes.size(); ++i) {
        for (int j = 0; j < second.size(); ++j) {
            if (firstSelectedGenes[i] == second[j])
                posSecondGenes.push_back(j);
        }
    }
    sort(posSecondGenes.begin(), posSecondGenes.end());
    for (int i = 0; i < posSecondGenes.size(); ++i) {
        second[posSecondGenes[i]] = firstSelectedGenes[i];
    }
}

void Memetic::mocCross(const int cut, const Chromosome &first, Chromosome &second) {
    vector<int> firstSelectedGenes;
    vector<int> posSecondGenes;
    for (int i = cut; i < first.size(); ++i) {
        firstSelectedGenes.push_back(first[i]);
    }
    for (int i = 0; i < firstSelectedGenes.size(); ++i) {
        for (int j = 0; j < second.size(); ++j) {
            if (firstSelectedGenes[i] == second[j])
                posSecondGenes.push_back(j);
        }
    }
    sort(posSecondGenes.begin(), posSecondGenes.end());
    for (int i = 0; i < posSecondGenes.size(); ++i) {
        second[posSecondGenes[i]] = firstSelectedGenes[i];
    }
}

void Memetic::mutation() {
    _log += "________Mutation________\n\n";
    int chance, rand1, rand2;
    vector<int> posToChange;
    for (int i = 0; i < _populationToEvolve; ++i) {
        for (int j = 0; j < _chromosomeSize; ++j) {
            chance = rand() % 100;
            if (chance >= _mutationChance) {
                _log += "Mutation individual " + to_string(i) + "\n";
                _currentPopulation[i].evaluated = false;
                rand1 = rand2 = j;
                while (rand1 == rand2 && rand2 == j && rand1 == j) {
                    rand1 = rand() % _chromosomeSize;
                    rand2 = rand() % _chromosomeSize;
                }
                posToChange.insert(posToChange.end(), {j, rand1, rand2});
                Utils::logVector("Before", &_currentPopulation[i].chromosome, _log);
                sort(posToChange.begin(), posToChange.end());
                swap(_currentPopulation[i].chromosome[posToChange[0]],
                     _currentPopulation[i].chromosome[posToChange[1]]);
                swap(_currentPopulation[i].chromosome[posToChange[1]],
                     _currentPopulation[i].chromosome[posToChange[2]]);
                Utils::logVector("After", &_currentPopulation[i].chromosome, _log);
                _log += "\n\n";
                posToChange.clear();
            }
        }
    }
}


void Memetic::reEval() {
    for (int i = 0; i < _populationToEvolve; ++i) {
        if (_currentPopulation[i].evaluated == false) {
            _currentPopulation[i].cost = Utils::solutionCost(_currentPopulation[i].chromosome,
                                                             _airport->getFluxMatrix(),
                                                             _airport->getDistanceMatrix(),
                                                             _airport->isSimetric());
            _currentPopulation[i].evaluated = true;
            _numEvaluations++;
        }
    }
}

void Memetic::selectRabble(vector<int> &rabblePositions, const int numRabbles) {
    vector<pair<int, int>> rabbleCostAndPos;
    for (int i = 0; i < numRabbles; ++i) {
        rabbleCostAndPos.push_back(make_pair(_initialPopulation[i].cost, i));
    }
    int posBest = Utils::posMinElement(rabbleCostAndPos);
    for (int i = numRabbles; i < _popSize; ++i) {
        if (_initialPopulation[i].cost > rabbleCostAndPos[posBest].first) {
            bool alreadyIn = false;
            for (int j = 0; j < numRabbles; ++j) {
                alreadyIn = checkIfSame(_initialPopulation[rabbleCostAndPos[j].second].chromosome,
                                        _currentPopulation[j].chromosome);
                if (alreadyIn)
                    break;
            }
            if (alreadyIn) continue;
            rabbleCostAndPos[posBest] = make_pair(_initialPopulation[i].cost, i);
            posBest = Utils::posMinElement(rabbleCostAndPos);
        }
    }
    for (int i = 0; i < numRabbles; ++i) {
        rabblePositions.push_back(rabbleCostAndPos[i].second);
    }
}

void Memetic::replacement() {
    vector<int> rabblePos;
    selectRabble(rabblePos, _populationToEvolve);
    for (int i = 0; i < _populationToEvolve; ++i) {
        _initialPopulation[rabblePos[i]].chromosome = _currentPopulation[i].chromosome;
        _initialPopulation[rabblePos[i]].cost = _currentPopulation[i].cost;
        _initialPopulation[rabblePos[i]].evaluated = _currentPopulation[i].evaluated;
    }
    _log += "\n\n";
}

bool Memetic::checkIfSame(const Chromosome &first, const Chromosome &second) {
    for (int i = 0; i < first.size(); ++i) {
        if (first[i] != second[i])
            return false;
    }
    return true;
}

const string &Memetic::getLog() const {
    return _log;
}

const Airport *Memetic::getAirport() const {
    return _airport;
}

const Chromosome &Memetic::getBestSolution() const {
    return _bestSolution;
}

double Memetic::getExecutionTime() const {
    return _executionTime;
}

int Memetic::getBestCost() const {
    return _bestCost;
}



