#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>


#include "data_structures/Graph.h"
#include "data_handler/data_handler.h"
#include "data_handler/csv_file.h"

Graph<int> graph;


void startMenu() {
    std::string option, graphstr;

    std::cout<<"INDIVIDUAL ROUTE PLANNING TOOL"<<std::endl;
    std::cout<<""<<std::endl;
    std::cout <<"Which graph would you like?(csv/simplified):";
    getline(std::cin, graphstr);

    if (graphstr == "csv") createGraph(graph,"Locations.csv", "Distances.csv");
    if (graphstr == "simplified") createGraph(graph, "LocationsSimplified.csv", "DistancesSimplified.csv");

    std::cout<<"Which option would you like to use?"<<std::endl;
    std::cout<<""<<std::endl;
    std::cout<<"Read the data from input"<<std::endl;
    std::cout<<"Read the data from a file"<<std::endl;
    std::cout<<""<<std::endl;
    std::cout<<"Option(file or input):";

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
}

int main(){
    startMenu();
    return 0;
}