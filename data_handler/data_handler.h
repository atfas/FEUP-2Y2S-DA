#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

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
void drivingFileInput();
void drivingIndependentOutput(int source, int destination, Path& routePath, Path& alternativePath);
void drivingRestrictedOutput(int source, int destination, Path& routePath);
Path createPath(std::vector<int> &path);
inline Path mergePaths(Path firstPath, Path secondPath) {
    Path result;
    if (firstPath.getRoute()[firstPath.getRoute().size()-1] == secondPath.getRoute()[0]){
        std::vector<int> newRoute = firstPath.getRoute();
        for (int i = 1; i < secondPath.getRoute().size(); i++) {
            newRoute.push_back(secondPath.getRoute()[i]);
        }
        result.setRoute(newRoute);
        result.setDistance(firstPath.getDistance() + secondPath.getDistance());
    }
    return result;
}
std::vector<std::pair<int, int>> parseAvoidSegments(const std::string& avoidSegmentsInput);

#endif
