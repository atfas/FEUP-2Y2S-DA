#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "data_structures/Graph.h"
#include "data_handler/data_handler.h"

Graph<int> graph;

void createGraph(Graph<int> &myGraph) {
    std::vector<int> parking = {2, 3, 7, 6};

    for (int i = 1; i <= 8; i++) {
        myGraph.addVertex(i);

        Vertex<int>* vertex = myGraph.findVertex(i);
        if (vertex && std::find(parking.begin(), parking.end(), i) != parking.end()) {
            vertex->setParking(true);
        }
    }

    myGraph.addEdge(1, 2, 10, 20);
    myGraph.addEdge(1, 3, INF, 15);
    myGraph.addEdge(2, 3, 5, 8);
    myGraph.addEdge(2, 4, 8, 25);
    myGraph.addEdge(3, 5, 12, 10);
    myGraph.addEdge(3, 7, 20, 25);
    myGraph.addEdge(4, 7, 30, 50);
    myGraph.addEdge(4, 8, 6, 18);
    myGraph.addEdge(4, 6, 15, 30);
    myGraph.addEdge(5, 6, INF, 10);
    myGraph.addEdge(6, 7, INF, 12);
    myGraph.addEdge(7, 8, 14, 20);
}


void startMenu() {
    std::string option;

    std::cout<<"INDIVIDUAL ROUTE PLANNING TOOL"<<std::endl;
    std::cout<<""<<std::endl;
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
        drivingFileInput();
    }
    else{
        std::cout<<"Invalid input"<<std::endl;
    }
}

int main(){

    createGraph(graph);
    startMenu();

    return 0;
}