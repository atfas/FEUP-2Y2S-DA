cmake_minimum_required(VERSION 3.30)
project(DA_Project_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(DA_Project_1 main.cpp
        data_structures/Graph.h
        Dijkstra.cpp
        data_structures/MutablePriorityQueue.h
        data_handler/data_handler.cpp
        data_handler/data_handler.h
        driving/driving.h
        driving/driving.cpp
)
