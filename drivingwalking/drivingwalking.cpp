#include "drivingwalking.h"
#include <unordered_map>
#include "../Dijkstra.cpp"

extern Graph<int> graph;

std::vector<std::pair<Path, Path>> findPaths(int source, int destination, int maxWalkTime,
    const std::vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}) {

    std::vector<std::pair<Path, Path>> paths;

    Dijkstra(&graph, source, avoidNodes, avoidSegments);

    std::unordered_map<int, Path> drivingPaths;

    for (auto v : graph.getVertexSet()) {
        if (v->isParking()) {
            std::vector<int> firstPath = getPath(&graph, source, v->getInfo());
            Path routePath = createPath(firstPath);
            drivingPaths[v->getInfo()] = routePath;
        }
    }

    Dijkstra(&graph, destination, avoidNodes, avoidSegments, true);

    std::unordered_map<int, Path> walkingPaths;

    for (auto v : graph.getVertexSet()) {
        if (v->isParking()) {
            std::vector<int> firstPath = getPath(&graph, destination, v->getInfo());
            std::reverse(firstPath.begin(), firstPath.end());
            Path routePath = createPath(firstPath, true);

            if (routePath.getDistance() <= maxWalkTime) {
                walkingPaths[v->getInfo()] = routePath;
            }
        }
    }

    if (walkingPaths.size() <= 1) {
        paths.push_back({{}, {}});
        paths.push_back({{}, {}});
    }

    for (const auto& [parkingNode, drivePath] : drivingPaths) {
        auto it = walkingPaths.find(parkingNode);
        if (it != walkingPaths.end()) {
            paths.push_back({drivePath, it->second});
        }
    }

    std::sort(paths.begin(), paths.end(), [](const std::pair<Path, Path>& a, const std::pair<Path, Path>& b) {
        double totalDistA = a.first.getDistance() + a.second.getDistance();
        double totalDistB = b.first.getDistance() + b.second.getDistance();
        if (totalDistA != totalDistB) return totalDistA < totalDistB;
        return a.second.getDistance() > b.second.getDistance();
    });

    return paths;
}


void drivingWalking(int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}) {
    auto paths = findPaths(source, destination, maxWalkTime, avoidNodes, avoidSegments);

    Path drivingPath = paths[0].first;
    Path walkingPath = paths[0].second;
    int parkingNode = -1;

    if (!walkingPath.getRoute().empty()) parkingNode = walkingPath.getRoute()[0];

    drivingWalkingOutput(source, destination, parkingNode, drivingPath, walkingPath, avoidNodes, avoidSegments, maxWalkTime);
}

void drivingWalkingApprox(int source, int destination, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>&
                    avoidSegments) {
    auto paths = findPaths(source, destination,999999, avoidNodes, avoidSegments);

    Path drivingPath1 = paths[0].first;
    Path walkingPath1 = paths[0].second;
    int parkingNode1 = -1;

    if (!walkingPath1.getRoute().empty()) parkingNode1 = walkingPath1.getRoute()[0];

    Path drivingPath2 = paths[1].first;
    Path walkingPath2 = paths[1].second;
    int parkingNode2 = -1;

    if (!walkingPath2.getRoute().empty()) parkingNode2 = walkingPath2.getRoute()[0];

    drivingWalkingApproxOutput(source, destination, parkingNode1,drivingPath1,walkingPath1, parkingNode2, drivingPath2, walkingPath2);
}

