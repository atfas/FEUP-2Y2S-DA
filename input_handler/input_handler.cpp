#include "input_handler.h"



void drivingIndependentInput() {
    std::string source, destination;

    std::cout << "Source:";
    std::getline(std::cin, source);

    std::cout << "Destination:";
    std::getline(std::cin, destination);

    std::cout << "========================================" << std::endl;
    independentDriving(stoi(source), stoi(destination));
}

// Handles interactive input for restricted driving route planning
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

    while (std::getline(ss, node, ',')) { avoidNodesString.push_back(node); }

    for (const auto& i : avoidNodesString){ avoidNodes.push_back(stoi(i)); }

    std::cout << "AvoidSegments:";
    std::getline(std::cin, avoidSegmentsInput);
    std::vector<std::pair<int, int>> avoidSegments = parseAvoidSegments(avoidSegmentsInput);

    std::cout <<"IncludeNode:";
    std::getline(std::cin, includeNode);

    std::cout << "========================================" << std::endl;

    restrictedDriving(stoi(source), stoi(destination), avoidNodes, avoidSegments, includeNode);
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

// Interactive input handlers
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

void fileInput() {

    std::cout << "========================================" << std::endl;

    std::ifstream i_file("input.txt");
    if (!i_file) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string mode, source, destination, maxWalkTime, avoidNodesString, avoidSegmentsString, includeNode;
    int num_lines = 0;
    std::unordered_map<std::string, std::string*> keyMap = {
        {"Mode", &mode},
        {"Source", &source},
        {"Destination", &destination},
        {"MaxWalkTime", &maxWalkTime},
        {"AvoidNodes", &avoidNodesString},
        {"AvoidSegments", &avoidSegmentsString},
        {"IncludeNode", &includeNode}
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
        else if (num_lines > 3) restrictedDriving(stoi(source), stoi(destination), avoidNodes, avoidSegments, includeNode);
    }
    else if (mode == "driving-walking")
        drivingWalking(stoi(source), stoi(destination), stoi(maxWalkTime), avoidNodes, avoidSegments);
}

// Main input reading function
void readInput() {
    std::string mode;

    std::cout << "========================================" << std::endl;
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