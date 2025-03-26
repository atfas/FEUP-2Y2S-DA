#ifndef CSV_FILE_H
#define CSV_FILE_H
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "../data_structures/Graph.h"

class LocationData {
private:
    int id;
    std::string code;
    bool parking;

public:
    // Constructor
    LocationData(int id, const std::string& code, bool parking)
        : id(id), code(code), parking(parking) {}

    // Default constructor
    LocationData() : id(0), code(""), parking(false) {}

    // Getters
    int getId() const { return id; }
    std::string getCode() const { return code; }
    bool hasParking() const { return parking; }
};

struct EdgeData {
    std::string location1;
    std::string location2;
    double drivingTime;
    double walkingTime;

    EdgeData()
        : location1(""), location2(""), drivingTime(0.0), walkingTime(0.0) {}

    EdgeData(const std::string& loc1, const std::string& loc2, double driving, double walking)
        : location1(loc1), location2(loc2), drivingTime(driving), walkingTime(walking) {}

    std::string getLocation1() const { return location1; }
    std::string getLocation2() const { return location2; }
    double getDrivingTime() const { return drivingTime; }
    double getWalkingTime() const { return walkingTime; }
};

std::unordered_map<std::string, LocationData> readLocations(const std::string& filename);
std::vector<EdgeData> readEdges(const std::string& filename);
void createGraph(Graph<int>& graph, const std::string& locationsFile, const std::string& edgesFile);
//void createGraph(Graph<int> &myGraph);

#endif //CSV_FILE_H
