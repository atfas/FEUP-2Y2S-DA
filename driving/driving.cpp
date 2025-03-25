#include "driving.h"
#include "../Dijkstra.cpp"
#include "../data_structures/Graph.h"
#include "../data_handler/data_handler.h"


extern Graph<int> graph;

void independentDriving(int source, int destination) {
    Dijkstra(&graph, source);
    vector<int> firstPath = getPath(&graph, source, destination);

    Path routePath = createPath(firstPath);

    vector<int> avoidNodes;
    if (!firstPath.empty()) {
        for (size_t i = 1; i < firstPath.size() - 1; i++) {
            avoidNodes.push_back(firstPath[i]);
        }
    }

    std::vector<std::pair<int, int>> avoidSegments;
    avoidSegments.push_back({source,destination});

    Dijkstra(&graph, source, avoidNodes, avoidSegments);
    vector<int> secondPath = getPath(&graph, source, destination);

    Path alternativePath = createPath(secondPath);

    drivingIndependentOutput(source,destination,routePath, alternativePath);
}


void restrictedDriving(int source, int destination, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments, const std::string& includeNode) {
    Path routePath;
    if (includeNode.empty()) {
        Dijkstra(&graph, source, avoidNodes, avoidSegments);
        vector<int> firstPath = getPath(&graph, source, destination);
        routePath = createPath(firstPath);
    }
    else if (stoi(includeNode)) {
        Dijkstra(&graph, source, avoidNodes, avoidSegments);
        vector<int> firstPath = getPath(&graph, source, stoi(includeNode));
        Path routePath1 = createPath(firstPath);

        Dijkstra(&graph, stoi(includeNode), avoidNodes, avoidSegments);
        vector<int> secondPath = getPath(&graph, stoi(includeNode), destination);
        Path routePath2 = createPath(secondPath);

        routePath = mergePaths(routePath1, routePath2);
    }

    drivingRestrictedOutput(source, destination, routePath);
}

