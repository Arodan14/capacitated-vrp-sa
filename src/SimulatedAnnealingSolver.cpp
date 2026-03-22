#include "SimulatedAnnealingSolver.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

SimulatedAnnealingSolver::SimulatedAnnealingSolver(const VRPProblem& p, double startTemp, double endTemp, double coolingAlpha, int iterationsPerTemp)
    : problem(p), tMax(startTemp), tMin(endTemp), alpha(coolingAlpha), iterPerTemp(iterationsPerTemp) {
}

double SimulatedAnnealingSolver::randomUnit() const {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

int SimulatedAnnealingSolver::routeLoad(const Route& route) const {
    int load = 0;
    const std::vector<int>& routeNodes = route.getNodes();

    for (int i = 1; i < static_cast<int>(routeNodes.size()) - 1; i++) {
        load += problem.getDemand(routeNodes[i]);
    }

    return load;
}

double SimulatedAnnealingSolver::routeDistance(const Route& route) const {
    double total = 0.0;
    const std::vector<int>& routeNodes = route.getNodes();

    for (int i = 0; i < static_cast<int>(routeNodes.size()) - 1; i++) {
        total += problem.getDistance(routeNodes[i], routeNodes[i + 1]);
    }

    return total;
}

double SimulatedAnnealingSolver::routeCost(const Route& route) const {
    double cost = 0.0;
    int remainingLoad = routeLoad(route);
    const std::vector<int>& routeNodes = route.getNodes();

    for (int i = 0; i < static_cast<int>(routeNodes.size()) - 1; i++) {
        double edgeDistance = problem.getDistance(routeNodes[i], routeNodes[i + 1]);
        cost += edgeDistance * remainingLoad * VRPProblem::COST_PER_KM;

        if (routeNodes[i + 1] != 0) {
            remainingLoad -= problem.getDemand(routeNodes[i + 1]);
        }
    }

    return cost;
}

double SimulatedAnnealingSolver::totalDistance(const Solution& solution) const {
    double total = 0.0;
    const std::vector<Route>& routes = solution.getRoutes();

    for (int i = 0; i < static_cast<int>(routes.size()); i++) {
        total += routeDistance(routes[i]);
    }

    return total;
}

double SimulatedAnnealingSolver::totalCost(const Solution& solution) const {
    double total = 0.0;
    const std::vector<Route>& routes = solution.getRoutes();

    for (int i = 0; i < static_cast<int>(routes.size()); i++) {
        total += VRPProblem::COST_PER_TRUCK;
        total += routeCost(routes[i]);
    }

    return total;
}

bool SimulatedAnnealingSolver::isRouteValid(const Route& route) const {
    const std::vector<int>& routeNodes = route.getNodes();

    if (routeNodes.size() < 2) {
        return false;
    }

    if (routeNodes.front() != 0 || routeNodes.back() != 0) {
        return false;
    }

    if (routeLoad(route) > VRPProblem::VEHICLE_CAPACITY) {
        return false;
    }

    return true;
}

bool SimulatedAnnealingSolver::isSolutionFeasible(const Solution& solution) const {
    std::vector<int> seen(VRPProblem::TOTAL_NODES, 0);
    const std::vector<Route>& routes = solution.getRoutes();

    for (int i = 0; i < static_cast<int>(routes.size()); i++) {
        if (!isRouteValid(routes[i])) {
            return false;
        }

        const std::vector<int>& routeNodes = routes[i].getNodes();

        for (int j = 1; j < static_cast<int>(routeNodes.size()) - 1; j++) {
            int customerId = routeNodes[j];

            if (customerId <= 0 || customerId > VRPProblem::NUM_CUSTOMERS) {
                return false;
            }

            seen[customerId]++;
        }
    }

    for (int customerId = 1; customerId <= VRPProblem::NUM_CUSTOMERS; customerId++) {
        if (seen[customerId] != 1) {
            return false;
        }
    }

    return true;
}

Solution SimulatedAnnealingSolver::generateInitialSolution() const {
    Solution solution;
    std::vector<int> used(VRPProblem::TOTAL_NODES, 0);

    for (int vehicle = 0; vehicle < VRPProblem::ESTIMATED_VEHICLES_NEEDED; vehicle++) {
        Route route;
        int currentLoad = 0;
        int lastNode = 0;

        while (true) {
            int bestCustomer = -1;
            double bestDistance = 1e18;

            for (int customerId = 1; customerId <= VRPProblem::NUM_CUSTOMERS; customerId++) {
                if (used[customerId] == 1) {
                    continue;
                }

                if (currentLoad + problem.getDemand(customerId) > VRPProblem::VEHICLE_CAPACITY) {
                    continue;
                }

                if (problem.getDistance(lastNode, customerId) < bestDistance) {
                    bestDistance = problem.getDistance(lastNode, customerId);
                    bestCustomer = customerId;
                }
            }

            if (bestCustomer == -1) {
                break;
            }

            route.insertCustomerBeforeDepot(bestCustomer);
            used[bestCustomer] = 1;
            currentLoad += problem.getDemand(bestCustomer);
            lastNode = bestCustomer;
        }

        if (!route.isEmpty()) {
            solution.addRoute(route);
        }
    }

    for (int customerId = 1; customerId <= VRPProblem::NUM_CUSTOMERS; customerId++) {
        if (used[customerId] == 0) {
            Route route;
            route.insertCustomerBeforeDepot(customerId);
            solution.addRoute(route);
        }
    }

    solution.setTotalCost(totalCost(solution));
    return solution;
}

bool SimulatedAnnealingSolver::swapCustomers(Solution& solution) const {
    std::vector<Route>& routes = solution.getRoutes();

    if (routes.size() < 2) {
        return false;
    }

    int firstRoute = rand() % routes.size();
    int secondRoute = rand() % routes.size();

    if (firstRoute == secondRoute) {
        return false;
    }

    if (routes[firstRoute].customerCount() == 0 || routes[secondRoute].customerCount() == 0) {
        return false;
    }

    std::vector<int>& firstNodes = routes[firstRoute].getNodes();
    std::vector<int>& secondNodes = routes[secondRoute].getNodes();

    int firstPosition = 1 + rand() % routes[firstRoute].customerCount();
    int secondPosition = 1 + rand() % routes[secondRoute].customerCount();

    int firstCustomer = firstNodes[firstPosition];
    int secondCustomer = secondNodes[secondPosition];

    int firstLoad = routeLoad(routes[firstRoute]) - problem.getDemand(firstCustomer) + problem.getDemand(secondCustomer);
    int secondLoad = routeLoad(routes[secondRoute]) - problem.getDemand(secondCustomer) + problem.getDemand(firstCustomer);

    if (firstLoad > VRPProblem::VEHICLE_CAPACITY || secondLoad > VRPProblem::VEHICLE_CAPACITY) {
        return false;
    }

    firstNodes[firstPosition] = secondCustomer;
    secondNodes[secondPosition] = firstCustomer;

    return true;
}

bool SimulatedAnnealingSolver::relocateCustomer(Solution& solution) const {
    std::vector<Route>& routes = solution.getRoutes();

    if (routes.size() < 2) {
        return false;
    }

    int fromRoute = rand() % routes.size();
    int toRoute = rand() % routes.size();

    if (fromRoute == toRoute) {
        return false;
    }

    if (routes[fromRoute].customerCount() <= 1) {
        return false;
    }

    std::vector<int>& fromNodes = routes[fromRoute].getNodes();
    int fromPosition = 1 + rand() % routes[fromRoute].customerCount();
    int customer = fromNodes[fromPosition];

    if (routeLoad(routes[toRoute]) + problem.getDemand(customer) > VRPProblem::VEHICLE_CAPACITY) {
        return false;
    }

    int insertPosition = 1 + rand() % (routes[toRoute].customerCount() + 1);
    routes[fromRoute].removeCustomerAt(fromPosition);
    routes[toRoute].insertCustomerAt(insertPosition, customer);
    solution.removeEmptyRoutes();

    return true;
}

Solution SimulatedAnnealingSolver::generateNeighbor(const Solution& current) const {
    Solution neighbor = current;
    bool changed = false;

    if (rand() % 2 == 0) {
        changed = swapCustomers(neighbor);
    } else {
        changed = relocateCustomer(neighbor);
    }

    if (!changed) {
        neighbor.setTotalCost(current.getTotalCost());
        return neighbor;
    }

    neighbor.setTotalCost(totalCost(neighbor));
    return neighbor;
}

Solution SimulatedAnnealingSolver::solve(std::vector<ProgressPoint>& history) const {
    Solution current = generateInitialSolution();
    Solution best = current;
    double temperature = tMax;

    history.clear();
    history.push_back(ProgressPoint{ temperature, best.getTotalCost() });

    while (temperature > tMin) {
        for (int iteration = 0; iteration < iterPerTemp; iteration++) {
            Solution neighbor = generateNeighbor(current);

            if (!isSolutionFeasible(neighbor)) {
                continue;
            }

            double changeInCost = neighbor.getTotalCost() - current.getTotalCost();

            if (changeInCost < 0.0 || exp(-changeInCost / temperature) > randomUnit()) {
                current = neighbor;

                if (current.getTotalCost() < best.getTotalCost()) {
                    best = current;
                }
            }
        }

        history.push_back(ProgressPoint{ temperature, best.getTotalCost() });
        temperature *= alpha;
    }

    return best;
}

void SimulatedAnnealingSolver::printSummary(const Solution& solution) const {
    std::cout << "Vehicles Used: " << solution.getRoutes().size() << "\n";
    std::cout << "Total Cost: " << solution.getTotalCost() << "\n";
    std::cout << "Total Distance: " << totalDistance(solution) << " km\n";
}

void SimulatedAnnealingSolver::printSolution(const Solution& solution) const {
    printSummary(solution);
    std::cout << "\n";

    const std::vector<Route>& routes = solution.getRoutes();

    for (int i = 0; i < static_cast<int>(routes.size()); i++) {
        std::cout << "Vehicle " << i + 1 << " (Load " << routeLoad(routes[i]) << "): ";

        const std::vector<int>& routeNodes = routes[i].getNodes();

        for (int j = 0; j < static_cast<int>(routeNodes.size()); j++) {
            std::cout << routeNodes[j];

            if (j != static_cast<int>(routeNodes.size()) - 1) {
                std::cout << " -> ";
            }
        }

        std::cout << "\n";
    }
}
