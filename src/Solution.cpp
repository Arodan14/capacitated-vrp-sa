#include "Solution.h"

Solution::Solution() {
    totalCostValue = 0.0;
}

std::vector<Route>& Solution::getRoutes() {
    return routes;
}

const std::vector<Route>& Solution::getRoutes() const {
    return routes;
}

void Solution::addRoute(const Route& route) {
    routes.push_back(route);
}

void Solution::removeEmptyRoutes() {
    std::vector<Route> cleanedRoutes;

    for (int i = 0; i < static_cast<int>(routes.size()); i++) {
        if (!routes[i].isEmpty()) {
            cleanedRoutes.push_back(routes[i]);
        }
    }

    routes = cleanedRoutes;
}

double Solution::getTotalCost() const {
    return totalCostValue;
}

void Solution::setTotalCost(double value) {
    totalCostValue = value;
}
