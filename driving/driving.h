#ifndef DRIVING_H
#define DRIVING_H
#include <string>
#include <vector>
#include "../Dijkstra.h"
#include "../data_structures/Graph.h"
#include "../data_handler/data_handler.h"
#include "../output_handler/output_handler.h"



void independentDriving(int source, int destination);
void restrictedDriving(int source, int destination, const std::vector<int>& avoidNodes, const std::vector<std::pair<int, int>>& avoidSegments, const std::string& includeNode);


#endif //DRIVING_H

