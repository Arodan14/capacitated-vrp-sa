#ifndef VRP_PROBLEM_H
#define VRP_PROBLEM_H

#include <string>
#include <vector>
#include "Node.h"

class VRPProblem {
private:
    std::vector<Node> allNodes;
    Node nodeById[201];
    int demandById[201];
    double distanceMatrix[201][201];

    void buildDistanceMatrix();

public:
    static const int VEHICLE_CAPACITY = 100;
    static const int NUM_CUSTOMERS = 200;
    static const int TOTAL_NODES = 201;
    static const int ESTIMATED_VEHICLES_NEEDED = 33;

    static constexpr double COST_PER_TRUCK = 500.0;
    static constexpr double COST_PER_KM = 0.024;

    VRPProblem();

    bool loadFromCSV(const std::string& fileName);

    int getDemand(int nodeId) const;
    double getDistance(int from, int to) const;
    const Node& getNode(int id) const;
    const std::vector<Node>& getAllNodes() const;
};

#endif
