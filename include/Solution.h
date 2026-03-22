#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include "Route.h"

class Solution {
private:
    std::vector<Route> routes;
    double totalCostValue;

public:
    Solution();

    std::vector<Route>& getRoutes();
    const std::vector<Route>& getRoutes() const;

    void addRoute(const Route& route);
    void removeEmptyRoutes();

    double getTotalCost() const;
    void setTotalCost(double value);
};

#endif
