#ifndef DRIVINGWALKING_H
#define DRIVINGWALKING_H
#include <utility>
#include <vector>
#include "../data_handler/data_handler.h"

void drivingWalking(int source, int destination, int maxWalkTime, std::vector<int>& avoidNodes, std::vector<std::pair<int, int>>&
                    avoidSegments);
void drivingWalkingApprox(int source, int destination);
std::vector<std::pair<Path,Path>> findPaths (int source, int destination, int maxWalkTime, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments);
#endif //DRIVINGWALKING_H
