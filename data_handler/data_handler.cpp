#include "../data_structures/Graph.h"
#include "data_handler.h"
#include <unordered_map>
#include "../driving/driving.h"
#include "../drivingwalking/drivingwalking.h"

extern Graph<int> graph;

// Parses avoid segments from input string into vector of integer pairs
std::vector<std::pair<int, int>> parseAvoidSegments(const std::string& avoidSegmentsInput) {
    std::vector<std::pair<std::string, std::string>> avoidSegmentsString;
    std::vector<std::pair<std::string, std::string>> segments;
    std::vector<std::pair<int, int>> avoidSegments;

    std::stringstream ssSegments(avoidSegmentsInput);
    std::string segment;

    int i = 0;
    while (std::getline(ssSegments, segment, ')')) {
        if (segment.empty()) continue;

        std::stringstream segStream(segment.substr(1)); // Remove the '('
        std::string first, second;

        std::getline(segStream, first, ',');
        std::getline(segStream, second, ',');

        if (i > 0) first = first.substr(1, first.size() - 1);

        avoidSegmentsString.emplace_back(first, second);
        i++;
    }

    for (const auto& pair : avoidSegmentsString){ avoidSegments.emplace_back(stoi(pair.first), stoi(pair.second)); }
    return avoidSegments;
}

// Creates a path from a vector of nodes, with optional reversal
Path createPath(std::vector<int> &path, bool reverse) {
    if (path.empty()) return Path();

    Vertex<int>* lastVertext;

    if (reverse) lastVertext = graph.findVertex(path.front());
    else lastVertext = graph.findVertex(path.back());

    Path newPath = Path(path, lastVertext->getDist());
    return newPath;
}

// Merges two paths that share a common node
Path mergePaths(Path firstPath, Path secondPath) {
    Path result;
    if (firstPath.getRoute()[firstPath.getRoute().size()-1] == secondPath.getRoute()[0]){
        std::vector<int> newRoute = firstPath.getRoute();

        for (int i = 1; i < secondPath.getRoute().size(); i++) { newRoute.push_back(secondPath.getRoute()[i]); }

        result.setRoute(newRoute);
        result.setDistance(firstPath.getDistance() + secondPath.getDistance());
    }
    return result;
}

// Writes path to terminal output
void writePathTerminal(Path &path) {
    if (path.getRoute().empty()) {
        std::cout<< "none" << std::endl;
        return;
    }

    for (size_t i = 0; i < path.getRoute().size(); i++) {
        std::cout << path.getRoute()[i];
        if (i != path.getRoute().size() - 1) { std::cout << ","; }
    }
    std::cout << "(" << path.getDistance() << ")" << std::endl;
}

// Writes path to file output
void writePathToFile(std::ofstream& file, Path &path) {
    if (path.getRoute().empty()) {
        file << "none\n";
        return;
    }

    for (size_t i = 0; i < path.getRoute().size(); i++) {
        file << path.getRoute()[i];

        if (i != path.getRoute().size() - 1) { file << ","; }
    }

    file << " (" << path.getDistance() << ")\n";
}

// Generates message for route impossibility scenarios
void messageOutput(std::string& message, int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}) {
    auto paths_maxTime = findPaths(source, destination, 999999, avoidNodes, avoidSegments);

    Path drivingPath_maxTime = paths_maxTime[0].first;
    Path walkingPath_maxTime = paths_maxTime[0].second;

    int parkingNode1 = -1;

    if (!walkingPath_maxTime.getRoute().empty()) parkingNode1 = walkingPath_maxTime.getRoute()[0];

    if (avoidNodes.empty() && avoidSegments.empty() || parkingNode1 != -1) {
        message = "No possible route with max. walking time of " + std::to_string(maxWalkTime) + " minutes.";
        return;
    }

    auto paths_blocked = findPaths(source, destination, maxWalkTime, {}, {});

    Path drivingPath_blocked = paths_blocked[0].first;
    Path walkingPath_blocked = paths_blocked[0].second;

    int parkingNode2 = -1;

    if (!walkingPath_blocked.getRoute().empty()) parkingNode2 = walkingPath_blocked.getRoute()[0];

    if (parkingNode2 != -1) {
        message = "No possible route avoiding the restricted nodes and edges.";
        return;
    }

    message = "No possible route with max. walking time of " + std::to_string(maxWalkTime) + " minutes and avoiding the restricted nodes and edges.";
}