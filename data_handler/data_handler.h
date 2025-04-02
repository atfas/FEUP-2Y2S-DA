#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include "../data_structures/Graph.h"

struct Path {
private:
    std::vector<int> route;
    double distance;
public:
    Path(){
        this->route = {};
        this->distance = INF;
    }
    Path(std::vector<int> route, double distance) {
        this->route = route;
        this->distance = distance;
    }
    std::vector<int> getRoute() {
        return route;
    }
    double getDistance() const {
        return distance;
    }
    void setDistance(double distance) {
        this->distance = distance;
    }
    void setRoute(std::vector<int> route){
        this->route = route;
    }
};

Path createPath(std::vector<int> &path, bool reverse = false);
Path mergePaths(Path firstPath, Path secondPath);
void writePathTerminal(Path &path);
void writePathToFile(std::ofstream& file, Path &path);
std::vector<std::pair<int, int>> parseAvoidSegments(const std::string& avoidSegmentsInput);
void messageOutput(std::string& message, int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments);
#endif
