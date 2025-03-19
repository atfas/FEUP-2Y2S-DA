#include "../data_structures/Graph.h"
#include "data_handler.h"
#include <unordered_map>
#include "../driving/driving.h"

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

    for (auto pair : avoidSegmentsString){
        avoidSegments.push_back({stoi(pair.first), stoi(pair.second)});
    }
    return avoidSegments;
}

Path createPath(std::vector<int> &path) {
    if (path.empty()) return Path();
    Vertex<int>* lastVertext = graph.findVertex(path.back());
    Path newPath = Path(path, lastVertext->getDist());
    return newPath;
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

    for (auto i : avoidNodesString){
        avoidNodes.push_back(stoi(i));
    }

    std::cout << "AvoidSegments:";
    std::getline(std::cin, avoidSegmentsInput);
    std::vector<std::pair<int, int>> avoidSegments = parseAvoidSegments(avoidSegmentsInput);

    std::cout <<"IncludeNode:";
    std::getline(std::cin, includeNode);

    restrictedDriving(stoi(source), stoi(destination), avoidNodes, avoidSegments, includeNode);
}

void drivingFileInput() {
    std::ifstream i_file("input.txt");
    if (!i_file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string mode, source, destination, avoidNodesString, avoidSegmentsString, includeNodes;
    int num_lines = 0;
    std::unordered_map<std::string, std::string*> keyMap = {
        {"Mode", &mode},
        {"Source", &source},
        {"Destination", &destination},
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


void readInput() {
    std::string mode;
    std::cout << "Mode:";
    getline(std::cin, mode);

    mode.erase(std::ranges::remove_if(mode, ::isspace).begin(), mode.end());
    if (mode == "driving") {
        inputDriving();
    }
    else if (mode == "driving-walking") {
        std::cout << "driving-walking" << std::endl;
    }
    else {
        std::cout << "Invalid input" << std::endl;
    }
}
