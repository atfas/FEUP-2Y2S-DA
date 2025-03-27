#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>


#include "data_structures/Graph.h"
#include "data_handler/data_handler.h"
#include "data_handler/csv_file.h"

Graph<int> graph;

// Display initial menu for graph and data input selection
void startMenu() {
    std::string option, graphstr;

    std::cout << "========================================" << std::endl;
    std::cout << "  INDIVIDUAL ROUTE PLANNING TOOL" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Select Graph Type:" << std::endl;
    std::cout << "  1. CSV Graph" << std::endl;
    std::cout << "  2. Simplified Graph" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter graph type (csv/simplified): ";
    getline(std::cin, graphstr);

    if (graphstr == "csv") createGraph(graph, "Locations.csv", "Distances.csv");
    if (graphstr == "simplified") createGraph(graph, "LocationsSimplified.csv", "DistancesSimplified.csv");

    std::cout << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "           DATA INPUT OPTIONS" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  1. Read data from input" << std::endl;
    std::cout << "  2. Read data from a file" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter option (file/input): ";

    getline(std::cin, option);

    option.erase(std::ranges::remove_if(option, ::isspace).begin(), option.end());
    std::ranges::transform(option, option.begin(), ::tolower);

    if (option == "input"){
        readInput();
    }
    else if (option == "file"){
        fileInput();
    }
    else{
        std::cout<<"Invalid input"<<std::endl;
    }

    std::cout << "========================================" << std::endl;

}

// Entry point of the application, initiates the start menu
int main(){
    startMenu();
    return 0;
}