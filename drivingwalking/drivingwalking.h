#ifndef DRIVINGWALKING_H
#define DRIVINGWALKING_H
#include <utility>
#include <vector>
#include <unordered_map>
#include "../data_handler/data_handler.h"
#include "../Dijkstra.h"
#include "../output_handler/output_handler.h"

void drivingWalking(int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments);
void drivingWalkingApprox(int source, int destination, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments);
std::vector<std::pair<Path,Path>> findPaths (int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments);
void drivingWalkingOutput(int source, int destination, int parkingNode, Path& drivingPath, Path& walkingPath, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments, int maxWalkTime);
#endif