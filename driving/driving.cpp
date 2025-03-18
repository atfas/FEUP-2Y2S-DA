//
// Created by ash on 3/18/25.
//
#include "../Dijkstra.cpp"
#include "../data_structures/Graph.h"
#include <sstream>

extern Graph<int> graph;

void printPath(vector<int> &path) {
    if (path.empty()) {
        std::cout<< "none" << std::endl;
        return;
    }

    for (size_t i = 0; i < path.size(); i++) {
        std::cout << path[i];
        if (i != path.size() - 1) {
            std::cout << ",";
        }
    }
    Vertex<int>* lastVertext = graph.findVertex(path.back());
    std::cout << "(" << lastVertext->getDist() << ")" << std::endl;
}

void independentDriving() {
    std::string source, destination;

    std::cout << "Source: ";
    std::getline(std::cin, source);

    std::cout << "Destination: ";
    std::getline(std::cin, destination);

    independentDijkstra(&graph, std::stoi(source));

    vector<int> routePath = getPath(&graph, stoi(source), stoi(destination));

    std::cout << "BestDrivingRoute:";
    printPath(routePath);

    vector<int> avoidNodes;
    for (size_t i = 1; i < routePath.size() -1; i++) {
        avoidNodes.push_back(routePath[i]);
    }

    independentDijkstra(&graph, std::stoi(source), avoidNodes);
    vector<int> alternativePath = getPath(&graph, stoi(source), stoi(destination));

    std::cout << "AlternativeDrivingRoute:";
    printPath(alternativePath);

}


void restrictedDriving() {
    std::string source, destination, avoidNodesInput, avoidSegmentsInput;
    std::vector<std::string> avoidNodes;
    std::vector<std::pair<std::string, std::string>> avoidSegments;

    std::cout << "Source: ";
    std::getline(std::cin, source);

    std::cout << "Destination: ";
    std::getline(std::cin, destination);

    std::cout << "AvoidNodes (comma-separated): ";
    std::getline(std::cin, avoidNodesInput);

    std::stringstream ss(avoidNodesInput);
    std::string node;

    while (std::getline(ss, node, ',')) {
        avoidNodes.push_back(node);
    }

    // Parsing avoid segments
    std::cout << "AvoidSegments ((id,id) format, comma-separated): ";
    std::getline(std::cin, avoidSegmentsInput);

    std::vector<std::pair<std::string, std::string>> segments;

    std::stringstream ssSegments(avoidSegmentsInput);
    std::string segment;

    while (std::getline(ssSegments, segment, ')')) {
        if (segment.empty()) continue; // Skip empty segments

        std::stringstream segStream(segment.substr(1)); // Remove the '('
        std::string first, second;

        std::getline(segStream, first, ',');  // Extract first value
        std::getline(segStream, second, ','); // Extract second value

        avoidSegments.emplace_back(first, second);
    }

    std::cout << "Parsed Avoid Segments: ";
    for (const auto& seg : avoidSegments) {
        std::cout << "(" << seg.first << "," << seg.second << ") ";
    }
}