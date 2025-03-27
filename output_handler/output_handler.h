#ifndef OUTPUT_HANDLER_H
#define OUTPUT_HANDLER_H

#include "../data_handler/data_handler.h"

void drivingIndependentOutput(int source, int destination, Path& routePath, Path& alternativePath);
void drivingRestrictedOutput(int source, int destination, Path& routePath);
void drivingWalkingApproxOutput(int source, int destination, int parkingNode1, Path& drivingPath1, Path& walkingPath1, int parkingNode2, Path& drivingPath2, Path& walkingPath2);

#endif //OUTPUT_HANDLER_H
