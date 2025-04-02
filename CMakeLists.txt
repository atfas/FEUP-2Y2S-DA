cmake_minimum_required(VERSION 3.30)
project(DA_Project_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(DA_Project_1 main.cpp
        data_structures/Graph.h
        data_structures/MutablePriorityQueue.h
        data_handler/data_handler.cpp
        data_handler/data_handler.h
        driving/driving.h
        driving/driving.cpp
        drivingwalking/drivingwalking.h
        drivingwalking/drivingwalking.cpp
        data_handler/csv_file.cpp
        data_handler/csv_file.h
        input_handler/input_handler.cpp
        input_handler/input_handler.h
        output_handler/output_handler.cpp
        output_handler/output_handler.h
        Dijkstra.h
)
