#include "data_handler.h"

void readInput() {
    std::string mode;
    std::cout << "Mode:";
    getline(std::cin, mode);

    mode.erase(std::ranges::remove_if(mode, ::isspace).begin(), mode.end());
    if (mode == "driving") {
        std::cout << "driving" << std::endl;
    }
    else if (mode == "driving-walking") {
        std::cout << "driving-walking" << std::endl;
    }
    else {
        std::cout << "Invalid input" << std::endl;
    }
}

void writeFile(const std::string& source, const std::string& destination){
    std::ofstream o_file("output.txt");
    if (!o_file) {
        std::cerr << "Error: Could not create file "<< std::endl;
    }
    o_file << "Source:" << source << "\n";
    o_file << "Destination:" << destination << "\n";
}

void readFile() {
    std::ifstream i_file("input.txt");
    if (!i_file) {
        std::cerr << "Error opening file!" << std::endl;
    }
    std::string mode, source, destination;

    std::string line;
    while (std::getline(i_file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            if (key == "Mode") {
                mode = value;
            } else if (key == "Source") {
                source = value;
            } else if (key == "Destination") {
                destination = value;
            }
        }
    }
    writeFile(source, destination);
}