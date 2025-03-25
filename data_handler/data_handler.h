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

void readInput();
void writeFile(const std::string& filename, const std::string& content);
void fileInput();
void drivingIndependentOutput(int source, int destination, Path& routePath, Path& alternativePath);
void drivingRestrictedOutput(int source, int destination, Path& routePath);
void drivingWalkingOutput(int source, int destination, int parkingNode, Path& drivingPath, Path& walkingPath, std::vector<int>& avoidNodes, std::
                          vector<std::pair<int, int>>& avoidSegments, int maxWalkTime);
Path createPath(std::vector<int> &path, bool reverse = false);
Path mergePaths(Path firstPath, Path secondPath);
std::vector<std::pair<int, int>> parseAvoidSegments(const std::string& avoidSegmentsInput);
void drivingWalkingApproxOutput(int source, int destination, int parkingNode1, Path& drivingPath1, Path& walkingPath1, int parkingNode2, Path& drivingPath2, Path& walkingPath2);

#endif
