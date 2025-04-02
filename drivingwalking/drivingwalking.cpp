#include "drivingwalking.h"

extern Graph<int> graph;

/**
 * Finds paths combining driving and walking routes
 * @param source Starting node ID
 * @param destination Ending node ID
 * @param maxWalkTime Maximum time allowed for walking
 * @param avoidNodes List of nodes to avoid in routing
 * @param avoidSegments List of road segments to avoid
 * @return Vector of driving and walking path pairs
 */
std::vector<std::pair<Path, Path>> findPaths(int source, int destination, int maxWalkTime,
    const std::vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}) {

    std::vector<std::pair<Path, Path>> paths;

    Dijkstra(&graph, source, avoidNodes, avoidSegments);

    std::unordered_map<int, Path> drivingPaths;

    for (auto v : graph.getVertexSet()) {
        if (v->isParking()) {
            std::vector<int> firstPath = getPath(&graph, source, v->getInfo());
            Path routePath = createPath(firstPath);
            drivingPaths[v->getInfo()] = routePath;
        }
    }

    Dijkstra(&graph, destination, avoidNodes, avoidSegments, true);

    std::unordered_map<int, Path> walkingPaths;

    for (auto v : graph.getVertexSet()) {
        if (v->isParking()) {
            std::vector<int> firstPath = getPath(&graph, destination, v->getInfo());
            std::reverse(firstPath.begin(), firstPath.end());
            Path routePath = createPath(firstPath, true);

            if (routePath.getDistance() <= maxWalkTime) {
                walkingPaths[v->getInfo()] = routePath;
            }
        }
    }

    if (walkingPaths.size() <= 1) {
        paths.push_back({{}, {}});
        paths.push_back({{}, {}});
    }

    for (const auto& [parkingNode, drivePath] : drivingPaths) {
        auto it = walkingPaths.find(parkingNode);
        if (it != walkingPaths.end()) {
            paths.push_back({drivePath, it->second});
        }
    }

    std::sort(paths.begin(), paths.end(), [](const std::pair<Path, Path>& a, const std::pair<Path, Path>& b) {
        double totalDistA = a.first.getDistance() + a.second.getDistance();
        double totalDistB = b.first.getDistance() + b.second.getDistance();
        if (totalDistA != totalDistB) return totalDistA < totalDistB;
        return a.second.getDistance() > b.second.getDistance();
    });

    return paths;
}


/**
 * Computes the best driving and walking route with a maximum walking time constraint
 * @param source Starting node ID
 * @param destination Ending node ID
 * @param maxWalkTime Maximum time allowed for walking
 * @param avoidNodes List of nodes to avoid in routing
 * @param avoidSegments List of road segments to avoid
 */
void drivingWalking(int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes = {}, const std::vector<std::pair<int, int>>& avoidSegments = {}) {
    auto paths = findPaths(source, destination, maxWalkTime, avoidNodes, avoidSegments);

    Path drivingPath = paths[0].first;
    Path walkingPath = paths[0].second;
    int parkingNode = -1;

    if (!walkingPath.getRoute().empty()) parkingNode = walkingPath.getRoute()[0];

    drivingWalkingOutput(source, destination, parkingNode, drivingPath, walkingPath, avoidNodes, avoidSegments, maxWalkTime);
}

/**
 * Computes approximation of two best driving and walking routes
 * @param source Starting node ID
 * @param destination Ending node ID
 * @param avoidNodes List of nodes to avoid in routing
 * @param avoidSegments List of road segments to avoid
 */
void drivingWalkingApprox(int source, int destination, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments) {
    auto paths = findPaths(source, destination,999999, avoidNodes, avoidSegments);

    Path drivingPath1 = paths[0].first;
    Path walkingPath1 = paths[0].second;
    int parkingNode1 = -1;

    if (!walkingPath1.getRoute().empty()) parkingNode1 = walkingPath1.getRoute()[0];

    Path drivingPath2 = paths[1].first;
    Path walkingPath2 = paths[1].second;
    int parkingNode2 = -1;

    if (!walkingPath2.getRoute().empty()) parkingNode2 = walkingPath2.getRoute()[0];

    drivingWalkingApproxOutput(source, destination, parkingNode1,drivingPath1,walkingPath1, parkingNode2, drivingPath2, walkingPath2);
}

void drivingWalkingOutput(int source, int destination, int parkingNode, Path& drivingPath, Path& walkingPath, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments, int maxWalkTime) {
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

        std::cout << "========================================" << std::endl;
        std::cout << "No route available. Would you like to see alternative routes? (Y/N):";
        std::getline(std::cin, answer);

        if (answer == "N") return;
        if (answer == "Y"){
            std::cout << "Source:" << source << std::endl;
            std::cout << "Destination:" << destination << std::endl;
            drivingWalkingApprox(source, destination, avoidNodes, avoidSegments);
        }

        std::cout << "========================================" << std::endl;
        std::cout << "Would you like to consider all nodes and edges for a more broad search?(Y/N):";
        std::getline(std::cin, answer);
        if (answer == "N") return;
        if (answer == "Y") drivingWalkingApprox(source, destination, {}, {});
    }
}