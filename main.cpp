#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

#include "ProgressPoint.h"
#include "SimulatedAnnealingSolver.h"
#include "Solution.h"
#include "SolutionVisualizer.h"
#include "VRPProblem.h"

int main(int argc, char* argv[]) {
    double tMax = 100000.0;
    double tMin = 1.0;
    double alpha = 0.995;
    int iter = 1000;
    unsigned int seed = static_cast<unsigned int>(time(NULL));
    std::string prefix = "";
    bool quiet = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--tmax" && i + 1 < argc) {
            tMax = atof(argv[++i]);
        } else if (arg == "--tmin" && i + 1 < argc) {
            tMin = atof(argv[++i]);
        } else if (arg == "--alpha" && i + 1 < argc) {
            alpha = atof(argv[++i]);
        } else if (arg == "--iter" && i + 1 < argc) {
            iter = atoi(argv[++i]);
        } else if (arg == "--seed" && i + 1 < argc) {
            seed = static_cast<unsigned int>(atoi(argv[++i]));
        } else if (arg == "--prefix" && i + 1 < argc) {
            prefix = argv[++i];
        } else if (arg == "--quiet") {
            quiet = true;
        }
    }

    srand(seed);

    VRPProblem problem;

    if (!problem.loadFromCSV("VRP(nodes).csv")) {
        std::cout << "Failed to read dataset file.\n";
        return 1;
    }

    SimulatedAnnealingSolver solver(problem, tMax, tMin, alpha, iter);
    std::vector<ProgressPoint> history;
    Solution bestSolution = solver.solve(history);

    if (quiet) {
        solver.printSummary(bestSolution);
    } else {
        solver.printSolution(bestSolution);
    }

    SolutionVisualizer visualizer(problem);
    std::string progressCsvFile = prefix + "annealing_progress.csv";
    std::string progressJsonFile = prefix + "annealing_progress.json";
    std::string solutionJsonFile = prefix + "solution.json";

    visualizer.saveProgressCSV(history, progressCsvFile);
    visualizer.saveProgressJSON(history, progressJsonFile);
    visualizer.saveSolutionJSON(bestSolution, solutionJsonFile);

    std::cout << "\nGenerated files:\n";
    std::cout << "- " << progressCsvFile << "\n";
    std::cout << "- " << progressJsonFile << "\n";
    std::cout << "- " << solutionJsonFile << "\n";

    return 0;
}
