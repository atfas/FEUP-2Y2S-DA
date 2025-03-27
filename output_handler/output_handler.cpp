#include "output_handler.h"

// Output functions for different routing scenarios
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
