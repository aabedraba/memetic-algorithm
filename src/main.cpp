#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "Utils.h"
#include "Airport.h"
#include "Genetic.h"

enum value {
    AIRPORT = 0,
    SEED = 1,
    CROSSOVER = 2,
    NUM_EVALS = 3,
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
    vector<string> airports = {"madrid03.dat"};
    vector<int> seeds = {23436383};
    string log;
    for (int j = 0; j < seeds.size(); ++j) {
        for (int i = 0; i < airports.size(); ++i) {
            string file = "../_data/" + airports[i];
            srand(seeds[j]);
            Airport *airport = new Airport(file);
            int numEvaluation = stoi(parameters[NUM_EVALS], nullptr, 10);
            Genetic *genetic = new Genetic(airport, parameters[CROSSOVER], numEvaluation);
            string type = "Genetic-" + parameters[CROSSOVER] + "-" + to_string(seeds[j]);
            Utils::getResults(type, airport->getAirportName(), genetic->getBestCost(), genetic->getBestSolution(), genetic->getExecutionTime(), log);
            //Utils::writeInFile(seeds[j], type, airport->getAirportName(), genetic->getLog());
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