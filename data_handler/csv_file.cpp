#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "../data_structures/Graph.h"
#include "csv_file.h"

std::unordered_map<std::string, LocationData> readLocations(const std::string& filename) {
    std::unordered_map<std::string, LocationData> locations;
    std::ifstream file(filename);
    std::string line, location, code;
    int id;
    bool parking;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return locations;
    }

    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::getline(ss, location, ',');
        ss >> id; ss.ignore();
        std::getline(ss, code, ',');
        ss >> parking;

        locations[code] = LocationData(id, code, parking);
    }

    file.close();
    return locations;
}


std::vector<EdgeData> readEdges(const std::string& filename) {
    std::vector<EdgeData> edges;
    std::ifstream file(filename);
    std::string line, loc1, loc2, drivingStr, walkingStr;
    double driving, walking;

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return edges;
    }

    std::getline(file, line);
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, loc1, ',');
        std::getline(ss, loc2, ',');
        std::getline(ss, drivingStr, ',');
        std::getline(ss, walkingStr, ',');

        // Handle "X" case for driving distance
        driving = (drivingStr == "X") ? INF : std::stod(drivingStr);
        walking = std::stod(walkingStr);

        edges.emplace_back(loc1, loc2, driving, walking);
    }

    file.close();
    return edges;
}


void createGraph(Graph<int>& graph, const std::string& locationsFile, const std::string& edgesFile) {
    std::unordered_map<std::string, LocationData> locations = readLocations(locationsFile);
    std::vector<EdgeData> edges = readEdges(edgesFile);
    std::unordered_map<std::string, int> locationToId;

    for (const auto& [location, data] : locations) {
        locationToId[location] = data.getId();
        graph.addVertex(data.getId());
        Vertex<int>* vertex = graph.findVertex(data.getId());
        if (vertex) {
            vertex->setParking(data.hasParking());
        }
    }

    for (const auto& edge : edges) {
        int id1 = locationToId[edge.getLocation1()];
        int id2 = locationToId[edge.getLocation2()];
        graph.addEdge(id1, id2, edge.getDrivingTime(), edge.getWalkingTime());
    }
}

