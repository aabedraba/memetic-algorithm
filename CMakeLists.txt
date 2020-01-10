cmake_minimum_required(VERSION 3.15)
project(3_memetic_algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(3_memetic_algorithms
        src/main.cpp
        src/Airport.cpp src/Airport.h
        src/Memetic.h src/Memetic.cpp
        src/Utils.h src/Utils.cpp
        src/TabuSearch.h src/TabuSearch.cpp)