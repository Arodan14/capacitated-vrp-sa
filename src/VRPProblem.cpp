#include "VRPProblem.h"

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>

VRPProblem::VRPProblem() {
    for (int i = 0; i < TOTAL_NODES; i++) {
        demandById[i] = 0;

        for (int j = 0; j < TOTAL_NODES; j++) {
            distanceMatrix[i][j] = 0.0;
        }
    }
}

bool VRPProblem::loadFromCSV(const std::string& fileName) {
    std::ifstream file(fileName.c_str());

    if (!file.is_open()) {
        return false;
    }

    allNodes.clear();

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        for (int i = 0; i < static_cast<int>(line.size()); i++) {
            if (line[i] == ';') {
                line[i] = ',';
            }
        }

        std::stringstream ss(line);
        std::string idText;
        std::string xText;
        std::string yText;
        std::string demandText;

        getline(ss, idText, ',');
        getline(ss, xText, ',');
        getline(ss, yText, ',');
        getline(ss, demandText, ',');

        if (idText.empty() || xText.empty() || yText.empty() || demandText.empty()) {
            continue;
        }

        Node node;
        node.id = atoi(idText.c_str());
        node.x = atoi(xText.c_str());
        node.y = atoi(yText.c_str());
        node.demand = atoi(demandText.c_str());

        if (node.id < 0 || node.id >= TOTAL_NODES) {
            continue;
        }

        allNodes.push_back(node);
        nodeById[node.id] = node;
        demandById[node.id] = node.demand;
    }

    if (static_cast<int>(allNodes.size()) != TOTAL_NODES) {
        return false;
    }

    buildDistanceMatrix();
    return true;
}

void VRPProblem::buildDistanceMatrix() {
    for (int i = 0; i < static_cast<int>(allNodes.size()); i++) {
        for (int j = 0; j < static_cast<int>(allNodes.size()); j++) {
            int firstId = allNodes[i].id;
            int secondId = allNodes[j].id;

            double dx = static_cast<double>(allNodes[i].x - allNodes[j].x);
            double dy = static_cast<double>(allNodes[i].y - allNodes[j].y);

            distanceMatrix[firstId][secondId] = sqrt(dx * dx + dy * dy);
        }
    }
}

int VRPProblem::getDemand(int nodeId) const {
    return demandById[nodeId];
}

double VRPProblem::getDistance(int from, int to) const {
    return distanceMatrix[from][to];
}

const Node& VRPProblem::getNode(int id) const {
    return nodeById[id];
}

const std::vector<Node>& VRPProblem::getAllNodes() const {
    return allNodes;
}
