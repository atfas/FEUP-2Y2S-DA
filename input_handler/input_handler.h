#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include "../data_structures/Graph.h"
#include "../data_handler/data_handler.h"
#include "../driving/driving.h"
#include "../drivingwalking/drivingwalking.h"

void readInput();
void fileInput();
void inputDriving();
void inputDrivingWalking();
void drivingRestrictedInput();
void drivingIndependentInput();


#endif //INPUT_HANDLER_H
