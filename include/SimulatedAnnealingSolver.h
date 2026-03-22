#ifndef SIMULATED_ANNEALING_SOLVER_H
#define SIMULATED_ANNEALING_SOLVER_H

#include <vector>
#include "ProgressPoint.h"
#include "Route.h"
#include "Solution.h"
#include "VRPProblem.h"

class SimulatedAnnealingSolver {
private:
    const VRPProblem& problem;
    double tMax;
    double tMin;
    double alpha;
    int iterPerTemp;

    double randomUnit() const;
    bool swapCustomers(Solution& solution) const;
    bool relocateCustomer(Solution& solution) const;
    Solution generateNeighbor(const Solution& current) const;

public:
    SimulatedAnnealingSolver(const VRPProblem& p, double startTemp, double endTemp, double coolingAlpha, int iterationsPerTemp);

    int routeLoad(const Route& route) const;
    double routeDistance(const Route& route) const;
    double routeCost(const Route& route) const;
    double totalDistance(const Solution& solution) const;
    double totalCost(const Solution& solution) const;
    bool isRouteValid(const Route& route) const;
    bool isSolutionFeasible(const Solution& solution) const;
    Solution generateInitialSolution() const;
    Solution solve(std::vector<ProgressPoint>& history) const;
    void printSummary(const Solution& solution) const;
    void printSolution(const Solution& solution) const;
};

#endif
