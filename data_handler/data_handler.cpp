#include "../data_structures/Graph.h"
#include "data_handler.h"
#include <unordered_map>
#include "../driving/driving.h"
#include "../drivingwalking/drivingwalking.h"

extern Graph<int> graph;

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

        std::getline(segStream, first, ',');  // Extract first value
        std::getline(segStream, second, ','); // Extract second value
        if (i > 0) first = first.substr(1, first.size() - 1);
        avoidSegmentsString.emplace_back(first, second);
        i++;
    }

    for (const auto& pair : avoidSegmentsString){
        avoidSegments.emplace_back(stoi(pair.first), stoi(pair.second));
    }
    return avoidSegments;
}

Path createPath(std::vector<int> &path, bool reverse) {
    if (path.empty()) return Path();
    Vertex<int>* lastVertext;
    if (reverse) lastVertext = graph.findVertex(path.front());
    else lastVertext = graph.findVertex(path.back());
    Path newPath = Path(path, lastVertext->getDist());
    return newPath;
}

Path mergePaths(Path firstPath, Path secondPath) {
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

void writePathTerminal(Path &path) {
    if (path.getRoute().empty()) {
        std::cout<< "none" << std::endl;
        return;
    }

    for (size_t i = 0; i < path.getRoute().size(); i++) {
        std::cout << path.getRoute()[i];
        if (i != path.getRoute().size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << "(" << path.getDistance() << ")" << std::endl;
}

void writePathToFile(std::ofstream& file, Path &path) {
    if (path.getRoute().empty()) {
        file << "none\n";
        return;
    }

    for (size_t i = 0; i < path.getRoute().size(); i++) {
        file << path.getRoute()[i];
        if (i != path.getRoute().size() - 1) {
            file << ",";
        }
    }

    file << " (" << path.getDistance() << ")\n";
}

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


void drivingIndependentInput() {
    std::string source, destination;

    std::cout << "Source:";
    std::getline(std::cin, source);

    std::cout << "Destination:";
    std::getline(std::cin, destination);

    independentDriving(stoi(source), stoi(destination));
}

void drivingRestrictedInput() {
    std::string source, destination, avoidNodesInput, avoidSegmentsInput, includeNode;
    std::vector<std::string> avoidNodesString;
    std::vector<int> avoidNodes;

    std::cout << "Source:";
    std::getline(std::cin, source);

    std::cout << "Destination:";
    std::getline(std::cin, destination);

    std::cout << "AvoidNodes:";
    std::getline(std::cin, avoidNodesInput);

    std::stringstream ss(avoidNodesInput);
    std::string node;

    while (std::getline(ss, node, ',')) {
        avoidNodesString.push_back(node);
    }

    for (const auto& i : avoidNodesString){
        avoidNodes.push_back(stoi(i));
    }

    std::cout << "AvoidSegments:";
    std::getline(std::cin, avoidSegmentsInput);
    std::vector<std::pair<int, int>> avoidSegments = parseAvoidSegments(avoidSegmentsInput);

    std::cout <<"IncludeNode:";
    std::getline(std::cin, includeNode);

    restrictedDriving(stoi(source), stoi(destination), avoidNodes, avoidSegments, includeNode);
}

void fileInput() {
    std::ifstream i_file("input.txt");
    if (!i_file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string mode, source, destination, maxWalkTime, avoidNodesString, avoidSegmentsString, includeNodes;
    int num_lines = 0;
    std::unordered_map<std::string, std::string*> keyMap = {
        {"Mode", &mode},
        {"Source", &source},
        {"Destination", &destination},
        {"MaxWalkTime", &maxWalkTime},
        {"AvoidNodes", &avoidNodesString},
        {"AvoidSegments", &avoidSegmentsString},
        {"IncludeNodes", &includeNodes}
    };

    std::string line;
    while (std::getline(i_file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            auto it = keyMap.find(key);
            if (it != keyMap.end()) {
                *(it->second) = value;
            }
        }
        num_lines++;
    }

    std::stringstream ss(avoidNodesString);
    std::string node;
    std::vector<int> avoidNodes;

    while (std::getline(ss, node, ',')) {
        try {
            avoidNodes.push_back(std::stoi(node));
        } catch (const std::exception& e) {
            std::cerr << "Invalid node ID: " << node << std::endl;
        }
    }

    std::vector<std::pair<int, int>> avoidSegments = parseAvoidSegments(avoidSegmentsString);

    i_file.close();
    if (mode == "driving") {
        if (num_lines == 3) independentDriving(stoi(source), stoi(destination));
        else if (num_lines > 3) restrictedDriving(stoi(source), stoi(destination), avoidNodes, avoidSegments, includeNodes);
    }
    else if (mode == "driving-walking") drivingWalking(stoi(source), stoi(destination), stoi(maxWalkTime), avoidNodes,
                                                       avoidSegments);
}


void drivingIndependentOutput(int source, int destination, Path& routePath, Path& alternativePath) {
    std::cout << "Source:" << source << std::endl;
    std::cout << "Destination:" << destination << std::endl;
    std::cout << "BestDrivingRoute:";
    writePathTerminal(routePath);
    std::cout << "AlternativeDrivingRoute:";
    writePathTerminal(alternativePath);

    std::ofstream o_file("output.txt");
    if (!o_file) {
        std::cerr << "Error: Could not create file" << std::endl;
        return;
    }
    o_file << "Source: " << source << "\n";
    o_file << "Destination: " << destination << "\n";

    o_file << "BestDrivingRoute: ";
    writePathToFile(o_file, routePath);

    o_file << "AlternativeDrivingRoute: ";
    writePathToFile(o_file, alternativePath);

    o_file.close();
}

void drivingRestrictedOutput(int source, int destination, Path& routePath) {
    std::cout << "Source:" << source << std::endl;
    std::cout << "Destination:" << destination << std::endl;
    std::cout << "RestrictedDrivingRoute:";
    writePathTerminal(routePath);

    std::ofstream o_file("output.txt");
    if (!o_file) {
        std::cerr << "Error: Could not create file" << std::endl;
        return;
    }
    o_file << "Source:" << source << "\n";
    o_file << "Destination:" << destination << "\n";

    o_file << "RestrictedDrivingRoute:";
    writePathToFile(o_file, routePath);

    o_file.close();
}



void drivingWalkingOutput(int source, int destination, int parkingNode, Path& drivingPath, Path& walkingPath, const std::vector<int>& avoidNodes, const std::
                          vector<std::pair<int, int>>& avoidSegments, int maxWalkTime) {
    std::string message;
    std::cout << "Source:" << source << std::endl;
    std::cout << "Destination:" << destination << std::endl;
    std::cout << "DrivingRoute:";
    writePathTerminal(drivingPath);

    if (parkingNode == -1) std::cout << "ParkingNode:none" << std::endl;
    else std::cout << "ParkingNode:" << parkingNode << std::endl;

    std::cout << "WalkingRoute:";
    writePathTerminal(walkingPath);

    if (walkingPath.getDistance() + drivingPath.getDistance() < INF) std::cout << "TotalTime:" << walkingPath.getDistance() + drivingPath.getDistance() << std::endl;
    else std::cout << "TotalTime:"<< std::endl;

    if (parkingNode == -1) messageOutput(message, source, destination, maxWalkTime, avoidNodes, avoidSegments);

    std::cout << "Message:" << message << std::endl;

    std::ofstream o_file("output.txt");
    if (!o_file) {
        std::cerr << "Error: Could not create file" << std::endl;
        return;
    }
    o_file << "Source:" << source << "\n";
    o_file << "Destination:" << destination << "\n";
    o_file << "DrivingRoute:";
    writePathToFile(o_file,drivingPath);

    if (parkingNode == -1) o_file << "ParkingNode:none" << "\n";
    else o_file << "ParkingNode:" << parkingNode << "\n";

    o_file << "WalkingRoute:";
    writePathToFile(o_file,walkingPath);

    if (walkingPath.getDistance() + drivingPath.getDistance() < INF) o_file << "TotalTime:" << walkingPath.getDistance() + drivingPath.getDistance() << "\n";
    else o_file << "TotalTime:" << "\n";

    o_file << "Message:" << message << "\n";

    o_file.close();

    if (parkingNode == -1) {
        std::string answer;
        std::cout << "No route available. Would you like to see alternative routes? (Y/N):";
        std::getline(std::cin, answer);

        if (answer == "N") return;
        if (answer == "Y"){
            std::cout << "Source:" << source << std::endl;
            std::cout << "Destination:" << destination << std::endl;
            drivingWalkingApprox(source, destination, avoidNodes, avoidSegments);
        }
        std::cout << "Would you like to consider all nodes and edges for a more broad search?(Y/N):";
        std::getline(std::cin, answer);
        if (answer == "N") return;
        if (answer == "Y") drivingWalkingApprox(source, destination, {}, {});
    }
}

void drivingWalkingApproxOutput(int source, int destination, int parkingNode1, Path& drivingPath1, Path& walkingPath1, int parkingNode2, Path& drivingPath2, Path& walkingPath2) {
    std::cout << "DrivingRoute1:";
    writePathTerminal(drivingPath1);

    if (parkingNode1 == -1) std::cout << "ParkingNode1:none" << std::endl;
    else std::cout << "ParkingNode1:" << parkingNode1 << std::endl;

    std::cout << "WalkingRoute1:";
    writePathTerminal(walkingPath1);

    if (walkingPath1.getDistance() + drivingPath1.getDistance() < INF) std::cout << "TotalTime1:" << walkingPath1.getDistance() + drivingPath1.getDistance() << std::endl;
    else std::cout << "TotalTime1:"<< std::endl;

    std::cout << "DrivingRoute2:";
    writePathTerminal(drivingPath2);

    if (parkingNode2 == -1) std::cout << "ParkingNode2:none" << std::endl;
    else std::cout << "ParkingNode2:" << parkingNode2 << std::endl;

    std::cout << "WalkingRoute2:";
    writePathTerminal(walkingPath2);

    if (walkingPath2.getDistance() + drivingPath2.getDistance() < INF) std::cout << "TotalTime2:" << walkingPath2.getDistance() + drivingPath2.getDistance() << std::endl;
    else std::cout << "TotalTime2:"<< std::endl;

    std::ofstream o_file("output.txt");
    if (!o_file) {
        std::cerr << "Error: Could not create file" << std::endl;
        return;
    }
    o_file << "Source:" << source << "\n";
    o_file << "Destination:" << destination << "\n";
    o_file << "DrivingRoute1:";
    writePathToFile(o_file,drivingPath1);

    if (parkingNode1 == -1) o_file << "ParkingNode1:none" << "\n";
    else o_file << "ParkingNode1:" << parkingNode1 << "\n";

    o_file << "WalkingRoute1:";
    writePathToFile(o_file,walkingPath1);

    if (walkingPath1.getDistance() + drivingPath1.getDistance() < INF) o_file << "TotalTime1:" << walkingPath1.getDistance() + drivingPath1.getDistance() << "\n";
    else o_file << "TotalTime1:" << "\n";

    o_file << "DrivingRoute2:";
    writePathToFile(o_file,drivingPath2);

    if (parkingNode2 == -1) o_file << "ParkingNode2:none" << "\n";
    else o_file << "ParkingNode2:" << parkingNode2 << "\n";

    o_file << "WalkingRoute2:";
    writePathToFile(o_file,walkingPath2);

    if (walkingPath2.getDistance() + drivingPath2.getDistance() < INF) o_file << "TotalTime2:" << walkingPath2.getDistance() + drivingPath2.getDistance() << "\n";
    else o_file << "TotalTime2:" << "\n";

    o_file.close();
}



void inputDriving() {
    std::string planning;
    std::cout << "Which type of planning would you like (independent/restricted): ";
    getline(std::cin, planning);

    if (planning == "independent") {
        drivingIndependentInput();
    }
    else if (planning == "restricted") {
        drivingRestrictedInput();
    }
    else std::cout << "Invalid planning input." << std::endl;
}

void inputDrivingWalking() {
    std::string source, destination, maxWalkTime, avoidNodesInput, avoidSegmentsInput;
    std::vector<std::string> avoidNodesString;
    std::vector<int> avoidNodes;

    std::cout << "Source:";
    std::getline(std::cin, source);

    std::cout << "Destination:";
    std::getline(std::cin, destination);

    std::cout << "MaxWalkTime:";
    std::getline(std::cin, maxWalkTime);

    std::cout << "AvoidNodes:";
    std::getline(std::cin, avoidNodesInput);

    std::stringstream ss(avoidNodesInput);
    std::string node;

    while (std::getline(ss, node, ',')) {
        avoidNodesString.push_back(node);
    }

    for (const auto& i : avoidNodesString){
        avoidNodes.push_back(stoi(i));
    }

    std::cout << "AvoidSegments:";
    std::getline(std::cin, avoidSegmentsInput);
    std::vector<std::pair<int, int>> avoidSegments = parseAvoidSegments(avoidSegmentsInput);

    drivingWalking(stoi(source), stoi(destination), stoi(maxWalkTime), avoidNodes, avoidSegments);
}


void readInput() {
    std::string mode;
    std::cout << "Mode:";
    getline(std::cin, mode);

    mode.erase(std::ranges::remove_if(mode, ::isspace).begin(), mode.end());
    if (mode == "driving") {
        inputDriving();
    }
    else if (mode == "driving-walking") {
        inputDrivingWalking();
    }
    else {
        std::cout << "Invalid input" << std::endl;
    }
}
