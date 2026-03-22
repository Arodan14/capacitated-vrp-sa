#ifndef SOLUTION_VISUALIZER_H
#define SOLUTION_VISUALIZER_H

#include <string>
#include <vector>
#include "ProgressPoint.h"
#include "Solution.h"
#include "VRPProblem.h"

class SolutionVisualizer {
private:
    const VRPProblem& problem;

public:
    SolutionVisualizer(const VRPProblem& p);

    void saveProgressCSV(const std::vector<ProgressPoint>& history, const std::string& fileName) const;
    void saveProgressJSON(const std::vector<ProgressPoint>& history, const std::string& fileName) const;
    void saveProgressSVG(const std::vector<ProgressPoint>& history, const std::string& fileName) const;
    void saveRoutesSVG(const Solution& solution, const std::string& fileName) const;
    void saveAnimatedRoutesSVG(const Solution& solution, const std::string& fileName) const;
    void saveSolutionJSON(const Solution& solution, const std::string& fileName) const;
};

#endif
