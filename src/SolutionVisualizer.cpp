#include "SolutionVisualizer.h"

#include <cmath>
#include <fstream>
#include <sstream>

SolutionVisualizer::SolutionVisualizer(const VRPProblem& p) : problem(p) {
}

void SolutionVisualizer::saveProgressJSON(const std::vector<ProgressPoint>& history, const std::string& fileName) const {
    std::ofstream file(fileName.c_str());

    if (!file.is_open()) {
        return;
    }

    file << "[";

    for (int i = 0; i < static_cast<int>(history.size()); i++) {
        if (i > 0) {
            file << ",";
        }

        file << "{\"step\":" << i << ",\"temperature\":" << history[i].temperature << ",\"bestCost\":" << history[i].bestCost << "}";
    }

    file << "]\n";
}

void SolutionVisualizer::saveSolutionJSON(const Solution& solution, const std::string& fileName) const {
    std::ofstream file(fileName.c_str());

    if (!file.is_open()) {
        return;
    }

    const std::vector<Node>& allNodes = problem.getAllNodes();

    file << "{\"nodes\":[";

    for (int i = 0; i < static_cast<int>(allNodes.size()); i++) {
        if (i > 0) {
            file << ",";
        }

        file << "{\"id\":" << allNodes[i].id << ",\"x\":" << allNodes[i].x << ",\"y\":" << allNodes[i].y << ",\"demand\":" << allNodes[i].demand << "}";
    }

    file << "],\"routes\":[";

    const std::vector<Route>& routes = solution.getRoutes();

    for (int r = 0; r < static_cast<int>(routes.size()); r++) {
        if (r > 0) {
            file << ",";
        }

        file << "[";

        const std::vector<int>& routeNodes = routes[r].getNodes();

        for (int j = 0; j < static_cast<int>(routeNodes.size()); j++) {
            if (j > 0) {
                file << ",";
            }

            file << routeNodes[j];
        }

        file << "]";
    }

    file << "]}\n";
}

void SolutionVisualizer::saveProgressCSV(const std::vector<ProgressPoint>& history, const std::string& fileName) const {
    std::ofstream file(fileName.c_str());

    if (!file.is_open()) {
        return;
    }

    file << "step,temperature,best_cost\n";

    for (int i = 0; i < static_cast<int>(history.size()); i++) {
        file << i << "," << history[i].temperature << "," << history[i].bestCost << "\n";
    }
}

void SolutionVisualizer::saveProgressSVG(const std::vector<ProgressPoint>& history, const std::string& fileName) const {
    if (history.empty()) {
        return;
    }

    double minCost = history[0].bestCost;
    double maxCost = history[0].bestCost;

    for (int i = 1; i < static_cast<int>(history.size()); i++) {
        if (history[i].bestCost < minCost) {
            minCost = history[i].bestCost;
        }

        if (history[i].bestCost > maxCost) {
            maxCost = history[i].bestCost;
        }
    }

    double width = 1000.0;
    double height = 500.0;
    double padding = 60.0;
    double usableWidth = width - 2.0 * padding;
    double usableHeight = height - 2.0 * padding;

    std::ofstream file(fileName.c_str());

    if (!file.is_open()) {
        return;
    }

    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" << width
         << "' height='" << height << "' viewBox='0 0 " << width << " " << height << "'>\n";
    file << "<rect width='100%' height='100%' fill='#f8fafc'/>\n";
    file << "<text x='60' y='35' font-size='24' fill='#0f172a'>Simulated Annealing Progress</text>\n";
    file << "<line x1='" << padding << "' y1='" << height - padding << "' x2='" << width - padding
         << "' y2='" << height - padding << "' stroke='#334155' stroke-width='2'/>\n";
    file << "<line x1='" << padding << "' y1='" << padding << "' x2='" << padding
         << "' y2='" << height - padding << "' stroke='#334155' stroke-width='2'/>\n";

    file << "<polyline fill='none' stroke='#2563eb' stroke-width='3' points='";

    for (int i = 0; i < static_cast<int>(history.size()); i++) {
        double x = padding;
        if (history.size() > 1) {
            x += usableWidth * i / (history.size() - 1);
        }

        double y = height / 2.0;
        if (maxCost > minCost) {
            y = padding + usableHeight * (maxCost - history[i].bestCost) / (maxCost - minCost);
        }

        file << x << "," << y;
        if (i != static_cast<int>(history.size()) - 1) {
            file << " ";
        }
    }

    file << "'/>\n";
    file << "<text x='" << padding << "' y='" << height - 20 << "' font-size='16' fill='#334155'>Cooling steps</text>\n";
    file << "<text x='10' y='" << padding << "' font-size='16' fill='#334155'>Best cost</text>\n";
    file << "</svg>\n";
}

void SolutionVisualizer::saveRoutesSVG(const Solution& solution, const std::string& fileName) const {
    const std::vector<Node>& allNodes = problem.getAllNodes();

    if (allNodes.empty()) {
        return;
    }

    int minX = allNodes[0].x;
    int maxX = allNodes[0].x;
    int minY = allNodes[0].y;
    int maxY = allNodes[0].y;

    for (int i = 1; i < static_cast<int>(allNodes.size()); i++) {
        if (allNodes[i].x < minX) {
            minX = allNodes[i].x;
        }
        if (allNodes[i].x > maxX) {
            maxX = allNodes[i].x;
        }
        if (allNodes[i].y < minY) {
            minY = allNodes[i].y;
        }
        if (allNodes[i].y > maxY) {
            maxY = allNodes[i].y;
        }
    }

    double width = 900.0;
    double height = 900.0;
    double padding = 50.0;
    double scaleX = (width - 2.0 * padding) / static_cast<double>(maxX - minX + 1);
    double scaleY = (height - 2.0 * padding) / static_cast<double>(maxY - minY + 1);

    std::string colors[10] = {
        "#2563eb", "#dc2626", "#16a34a", "#ea580c", "#7c3aed",
        "#0891b2", "#db2777", "#65a30d", "#4f46e5", "#b45309"
    };

    std::ofstream file(fileName.c_str());

    if (!file.is_open()) {
        return;
    }

    file << "<svg xmlns='http://www.w3.org/2000/svg' width='" << width
         << "' height='" << height << "' viewBox='0 0 " << width << " " << height << "'>\n";
    file << "<rect width='100%' height='100%' fill='#ffffff'/>\n";
    file << "<text x='40' y='35' font-size='24' fill='#111827'>Vehicle Routing Solution</text>\n";

    const std::vector<Route>& routes = solution.getRoutes();

    for (int routeIndex = 0; routeIndex < static_cast<int>(routes.size()); routeIndex++) {
        const std::vector<int>& routeNodes = routes[routeIndex].getNodes();
        std::string color = colors[routeIndex % 10];

        for (int i = 0; i < static_cast<int>(routeNodes.size()) - 1; i++) {
            Node first = problem.getNode(routeNodes[i]);
            Node second = problem.getNode(routeNodes[i + 1]);

            double x1 = padding + (first.x - minX) * scaleX;
            double y1 = height - padding - (first.y - minY) * scaleY;
            double x2 = padding + (second.x - minX) * scaleX;
            double y2 = height - padding - (second.y - minY) * scaleY;

            file << "<line x1='" << x1 << "' y1='" << y1
                 << "' x2='" << x2 << "' y2='" << y2
                 << "' stroke='" << color << "' stroke-width='2.5' opacity='0.8'/>\n";
        }
    }

    for (int i = 0; i < static_cast<int>(allNodes.size()); i++) {
        double x = padding + (allNodes[i].x - minX) * scaleX;
        double y = height - padding - (allNodes[i].y - minY) * scaleY;

        if (allNodes[i].id == 0) {
            file << "<circle cx='" << x << "' cy='" << y << "' r='7' fill='#111827'/>\n";
            file << "<text x='" << x + 10 << "' y='" << y - 10
                 << "' font-size='14' fill='#111827'>Depot</text>\n";
        } else {
            file << "<circle cx='" << x << "' cy='" << y << "' r='3.5' fill='#475569'/>\n";
        }
    }

    file << "</svg>\n";
}

void SolutionVisualizer::saveAnimatedRoutesSVG(const Solution& solution, const std::string& fileName) const {
    const std::vector<Node>& allNodes = problem.getAllNodes();

    if (allNodes.empty()) {
        return;
    }

    int minX = allNodes[0].x;
    int maxX = allNodes[0].x;
    int minY = allNodes[0].y;
    int maxY = allNodes[0].y;

    for (int i = 1; i < static_cast<int>(allNodes.size()); i++) {
        if (allNodes[i].x < minX) {
            minX = allNodes[i].x;
        }
        if (allNodes[i].x > maxX) {
            maxX = allNodes[i].x;
        }
        if (allNodes[i].y < minY) {
            minY = allNodes[i].y;
        }
        if (allNodes[i].y > maxY) {
            maxY = allNodes[i].y;
        }
    }

    double width = 980.0;
    double height = 920.0;
    double padding = 60.0;
    double scaleX = (width - 2.0 * padding) / static_cast<double>(maxX - minX + 1);
    double scaleY = (height - 2.0 * padding) / static_cast<double>(maxY - minY + 1);

    std::string colors[10] = {
        "#2563eb", "#dc2626", "#16a34a", "#ea580c", "#7c3aed",
        "#0891b2", "#db2777", "#65a30d", "#4f46e5", "#b45309"
    };

    std::ofstream file(fileName.c_str());

    if (!file.is_open()) {
        return;
    }

    file << "<svg xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' width='" << width
         << "' height='" << height << "' viewBox='0 0 " << width << " " << height << "'>\n";
    file << "<rect width='100%' height='100%' fill='#ffffff'/>\n";
    file << "<text x='40' y='38' font-size='26' fill='#111827'>Animated Final Route Playback</text>\n";
    file << "<text x='40' y='66' font-size='15' fill='#52607a'>Truck markers move along the best route solution found by Simulated Annealing.</text>\n";

    const std::vector<Route>& routes = solution.getRoutes();
    double totalAnimationTime = 0.0;

    for (int routeIndex = 0; routeIndex < static_cast<int>(routes.size()); routeIndex++) {
        const std::vector<int>& routeNodes = routes[routeIndex].getNodes();
        std::string color = colors[routeIndex % 10];
        std::ostringstream pathStream;
        double routeLength = 0.0;

        for (int i = 0; i < static_cast<int>(routeNodes.size()); i++) {
            Node node = problem.getNode(routeNodes[i]);
            double x = padding + (node.x - minX) * scaleX;
            double y = height - padding - (node.y - minY) * scaleY;

            if (i == 0) {
                pathStream << "M " << x << " " << y << " ";
            } else {
                pathStream << "L " << x << " " << y << " ";
                routeLength += problem.getDistance(routeNodes[i - 1], routeNodes[i]);
            }
        }

        double duration = 3.0 + routeLength / 45.0;
        double beginTime = totalAnimationTime;
        totalAnimationTime += duration + 0.3;

        file << "<path id='routePath" << routeIndex << "' d='" << pathStream.str()
             << "' fill='none' stroke='" << color << "' stroke-width='3' opacity='0.86' stroke-linecap='round' stroke-linejoin='round'/>\n";

        file << "<circle r='8' fill='" << color << "' stroke='#ffffff' stroke-width='2.5'>\n";
        file << "<animateMotion dur='" << duration << "s' begin='" << beginTime
             << "s' repeatCount='indefinite' rotate='auto'>\n";
        file << "<mpath xlink:href='#routePath" << routeIndex << "'/>\n";
        file << "</animateMotion>\n";
        file << "</circle>\n";

        file << "<text x='" << (width - 160) << "' y='" << (110 + routeIndex * 22)
             << "' font-size='13' fill='" << color << "'>Route " << routeIndex + 1
             << " | duration " << duration << "s</text>\n";
    }

    for (int i = 0; i < static_cast<int>(allNodes.size()); i++) {
        double x = padding + (allNodes[i].x - minX) * scaleX;
        double y = height - padding - (allNodes[i].y - minY) * scaleY;

        if (allNodes[i].id == 0) {
            file << "<circle cx='" << x << "' cy='" << y << "' r='10' fill='#111827'/>\n";
            file << "<circle cx='" << x << "' cy='" << y << "' r='18' fill='none' stroke='#94a3b8' stroke-width='2' stroke-dasharray='5 5'/>\n";
            file << "<text x='" << x + 14 << "' y='" << y - 14 << "' font-size='15' fill='#111827'>Depot</text>\n";
        } else {
            file << "<circle cx='" << x << "' cy='" << y << "' r='3.4' fill='#475569'/>\n";
        }
    }

    file << "</svg>\n";
}
