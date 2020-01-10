#include "Utils.h"
#include "Airport.h"
#include "Memetic.h"
#include "TabuSearch.h"

enum value {
    AIRPORT,
    SEED,
    S_CROSSOVER,
    S_NUM_EVALS,
    S_CROSSOVER_CHANCE,
    S_MUTATION_CHANCE,
    S_POPULATION_TO_EVOLVE,
    S_TOURNAMENT_SIZE,
    T_GENERATIONS,
    T_ITERATIONS_IN_SOLUTION,
    T_TABU_LIST_SIZE,
    T_TABU_ENVIROMENTS_TO_GENERATE
};

void readParametersFile(vector<string> &parameters) {
    ifstream file;
    file.open("../_parametros.txt");
    string linea;
    if (file.good()) {
        while (!file.eof()) {
            file.ignore(256, ' ');
            file.ignore(256, ' ');
            file >> linea;
            parameters.push_back(linea.c_str());
        }
    }
    file.close();
}


int main() {
    vector<string> parameters;
    readParametersFile(parameters);
    //TAKING ALL PARAMETERS HERE FOR FASTER DEVELOPMENT
    vector<string> airports = {"madrid01.dat", "madrid02.dat", "madrid03.dat", "madrid04.dat",
                               "malaga01.dat", "malaga02.dat", "malaga03.dat", "malaga04.dat"};
    vector<int> seeds = {70911512, 53597523, 53911043, 53916131, 70911512};
    string log;
    for (int j = 0; j < seeds.size(); ++j) {
        for (int i = 0; i < airports.size(); ++i) {
            string file = "../_data/" + airports[i];
            srand(seeds[j]);
            Airport *airport = new Airport(file);
            const int numEvaluation = stoi(parameters[S_NUM_EVALS], nullptr, 10);
            const int crossOverChance = stoi(parameters[S_CROSSOVER_CHANCE]);
            const int populationToEvolve = stoi(parameters[S_POPULATION_TO_EVOLVE]);
            const int mutationChance = stoi(parameters[S_MUTATION_CHANCE]);
            const int tournamentSize = stoi(parameters[S_TOURNAMENT_SIZE]);
            const int generations = stoi(parameters[T_GENERATIONS]);
            const int iterationsInSolution = stoi(parameters[T_ITERATIONS_IN_SOLUTION]);
            const int tabuListSize = stoi(parameters[T_TABU_LIST_SIZE]);
            const int enviromentsToGenerate = stoi(parameters[T_TABU_ENVIROMENTS_TO_GENERATE]);
            TabuSearch *tabu = new TabuSearch(airport, generations, iterationsInSolution, tabuListSize,
                                              enviromentsToGenerate);
            Memetic *genetic = new Memetic(airport, tabu, parameters[S_CROSSOVER], numEvaluation, populationToEvolve,
                                           crossOverChance, mutationChance, tournamentSize);
            string type =
                    "Memetic-" + parameters[S_CROSSOVER] + "-" + parameters[S_NUM_EVALS] + "-" + to_string(seeds[j]);
            Utils::getResults(type, airport->getAirportName(), genetic->getBestCost(), genetic->getBestSolution(),
                              genetic->getExecutionTime(), log);
            Utils::writeInFile(seeds[j], type, airport->getAirportName(), genetic->getLog());
            delete airport;
            delete genetic;
        }
    }

    ofstream fs;
    fs.open("../_logs/results.txt");
    if (fs.is_open()) {
        fs << log << endl;
    } else
        throw "File not properly opened!";


    return 0;
}